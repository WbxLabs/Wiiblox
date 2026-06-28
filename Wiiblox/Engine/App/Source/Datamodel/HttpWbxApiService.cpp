#include "stdafx.h"

#include "V8DataModel/HttpWbxApiService.h"
#include "V8DataModel/DataModel.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/Stats.h"
#include "v8datamodel/HttpWbxApiJob.h"
#include "Network/Players.h"
#include "V8Xml/WebParser.h"
#include "Util/WiibloxGoogleAnalytics.h"

#define HTTP_POST_COMPRESSION_LIMIT 256

DYNAMIC_FASTINTVARIABLE(PercentApiRequestsRecordGoogleAnalytics, 1)

DYNAMIC_FASTINTVARIABLE(HttpWbxApiClientPerMinuteRequestLimit, 300)
DYNAMIC_FASTINTVARIABLE(HttpWbxApiMaxBudgetMultiplier, 1)
DYNAMIC_FASTINTVARIABLE(HttpWbxApiRequestsPerMinuteServerLimit, 300)
DYNAMIC_FASTINTVARIABLE(HttpWbxApiRequestsPerMinutePerPlayerInServerLimit, 100)
DYNAMIC_FASTINTVARIABLE(HttpWbxApiMaxThrottledQueueSize, 50)

DYNAMIC_FASTINTVARIABLE(HttpWbxApiMaxRetryBudgetPerMinute, 500)
DYNAMIC_FASTINTVARIABLE(HttpWbxApiMaxRetryCount, 10)
DYNAMIC_FASTINTVARIABLE(HttpWbxApiMaxRetryQueueSize, 500)
DYNAMIC_FASTINTVARIABLE(HttpWbxApiMaxSyncRetries, 3)
DYNAMIC_FASTINTVARIABLE(HttpWbxApiSyncRetryWaitTimeMSec, 500)

DYNAMIC_FASTFLAG(UseR15Character)

LOGVARIABLE(HttpWbxApiBudget, 0);

namespace {
	static inline void sendApiServiceDidThrottle()
	{
		wbx::WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "GameHasBeenAPIThrottled");
	}
	static inline void sendApiServiceDidQueue()
	{
		wbx::WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "GameHasBeenAPIQueued");
	}
}

namespace WBX {
	std::string HttpWbxApiService::StaticApiBaseUrl;
	const char* const sHttpWbxApiService = "HttpWbxApiService";

    REFLECTION_BEGIN();
	// CoreScript Exposed Functions
	static Reflection::BoundYieldFuncDesc<HttpWbxApiService, std::string(std::string, bool, ThrottlingPriority)> apiGetAsyncFunction(&HttpWbxApiService::getAsyncLua, "GetAsync", "apiUrlPath", "useHttps", true, "priority", PRIORITY_DEFAULT, Security::WiibloxScript);
	static Reflection::BoundYieldFuncDesc<HttpWbxApiService, std::string(std::string, std::string, bool, ThrottlingPriority, HttpService::HttpContentType)> apiPostAsyncFunction(&HttpWbxApiService::postAsyncLua, "PostAsync", "apiUrlPath", "data", "useHttps", true, "priority", PRIORITY_DEFAULT, "content_type", HttpService::APPLICATION_JSON, Security::WiibloxScript);
    REFLECTION_END();

	namespace Reflection {
		template<>
		EnumDesc<ThrottlingPriority>::EnumDesc()
			:EnumDescriptor("ThrottlingPriority")
		{
			addPair(PRIORITY_EXTREME,			"Extreme");
			addPair(PRIORITY_SERVER_ELEVATED,	"ElevatedOnServer");
			addPair(PRIORITY_DEFAULT,			"Default");
		}

		template<>
		ThrottlingPriority& Variant::convert<ThrottlingPriority>(void)
		{
			return genericConvert<ThrottlingPriority>();
		}

	} // namespace Reflection

	template<>
	bool StringConverter<ThrottlingPriority>::convertToValue(const std::string& text, ThrottlingPriority& value)
	{
		return Reflection::EnumDesc<ThrottlingPriority>::singleton().convertToValue(text.c_str(), value);
	}


	HttpWbxApiService::HttpWbxApiService() :
		apiBaseUrl(""),
		serverPresent(false),
		clientPresent(false),
		isPlaySolo(false),
		totalNumOfApiCalls(0)
	{
		recordInGoogleAnalytics = rand() % 100 < DFInt::PercentApiRequestsRecordGoogleAnalytics;

		setName(sHttpWbxApiService);
	}

	void HttpWbxApiService::setStaticApiBaseUrl(const wbx::Reflection::PropertyDescriptor* pPropertyDescriptor)
	{
		if (*pPropertyDescriptor == wbx::ContentProvider::desc_baseUrl)
		{		
			contentProviderPropertyChangedConnection.disconnect();

			apiBaseUrl = ServiceProvider::create<ContentProvider>(this)->getApiBaseUrl();

			if (!apiBaseUrl.empty())
			{
				const std::string httpsString = "https";
				apiBaseUrl.replace(apiBaseUrl.find(httpsString),httpsString.length(),"");

				HttpWbxApiService::StaticApiBaseUrl = apiBaseUrl;

				if (DFFlag::UseR15Character)
				{
					if(DataModel* dataModel = DataModel::get(this)) 
					{
						dataModel->setCanRequestUniverseInfo(true);
					}
				}

			}
		}

	}

	void HttpWbxApiService::onServiceProvider(ServiceProvider* oldProvider, ServiceProvider* newProvider)
	{
		if (oldProvider)
		{
			if (getRecordInGoogleAnalytics())
			{
				if (isPlaySolo)
				{
					wbx::WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "TotalHttpApiCallsInPlaySolo", totalNumOfApiCalls);
					wbx::WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "AvgHttpApiCallsPerSecInPlaySolo",((double)totalNumOfApiCalls)/instanceAliveTimer.delta().seconds());
				}
				else if(serverPresent)
				{
					wbx::WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "TotalHttpApiCallsInServer",totalNumOfApiCalls);
					wbx::WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "AvgHttpApiCallsPerSecInServer",((double)totalNumOfApiCalls)/instanceAliveTimer.delta().seconds());
				}
				else if(clientPresent)
				{
					wbx::WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "TotalHttpApiCallsInClient",totalNumOfApiCalls);
					wbx::WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "AvgHttpApiCallsPerSecInClient",((double)totalNumOfApiCalls)/instanceAliveTimer.delta().seconds());
				}
			}

			disconnectEventConnections();

			serverPresent = false;
			clientPresent = false;
			isPlaySolo = false;

			TaskScheduler::singleton().removeBlocking(HttpWbxApiJob);
			HttpWbxApiJob.reset();
		}

		Super::onServiceProvider(oldProvider,newProvider);

		if (newProvider)
		{
			apiBaseUrl = ServiceProvider::create<ContentProvider>(newProvider)->getApiBaseUrl();
			instanceAliveTimer.reset();

			if (!apiBaseUrl.empty())
			{
				const std::string httpsString = "https";
				apiBaseUrl.replace(apiBaseUrl.find(httpsString),httpsString.length(),"");

				HttpWbxApiService::StaticApiBaseUrl = apiBaseUrl;
			}
			else
			{
				contentProviderPropertyChangedConnection = ServiceProvider::find<ContentProvider>(newProvider)->propertyChangedSignal.connect(boost::bind(&HttpWbxApiService::setStaticApiBaseUrl, this, _1));
			}

			// check to see if we are a client, server, or in play solo.
			// this may not return a mode immediately so set up connections
			// for verification later
			checkForClientAndServer(newProvider);
			if (!serverPresent && !clientPresent)
			{
				if (Network::Players* players = ServiceProvider::find<Network::Players>(newProvider))
				{
					playersChangedConnection = players->propertyChangedSignal.connect( boost::bind(&HttpWbxApiService::playersPropertyChanged, this, _1) );
				}

				serviceAddedConnection = newProvider->serviceAddedSignal.connect( boost::bind(&HttpWbxApiService::newServiceAdded, this, _1) );
			}

			HttpWbxApiJob.reset(new HttpWbxApiJob(this));
			TaskScheduler::singleton().add(HttpWbxApiJob);
		}
	}

	void HttpWbxApiService::disconnectEventConnections()
	{
		serviceAddedConnection.disconnect();
		playersChangedConnection.disconnect();
	}

	void HttpWbxApiService::playersPropertyChanged(const wbx::Reflection::PropertyDescriptor* desc)
	{
		if (!serverPresent && !clientPresent && desc == &Network::Players::propLocalPlayer)
		{
			// this should only be hit and is play solo because there is no client or server, but we have local player
			disconnectEventConnections();

			isPlaySolo = true;

			defaultServerThrottle.addBudget(DFInt::HttpWbxApiRequestsPerMinuteServerLimit, DFInt::HttpWbxApiRequestsPerMinuteServerLimit);
			elevatedServerThrottle.addBudget(DFInt::HttpWbxApiRequestsPerMinuteServerLimit, DFInt::HttpWbxApiRequestsPerMinuteServerLimit);
			clientThrottle.addBudget(DFInt::HttpWbxApiClientPerMinuteRequestLimit, DFInt::HttpWbxApiClientPerMinuteRequestLimit);
			retryBudget.addBudget(DFInt::HttpWbxApiMaxRetryBudgetPerMinute, DFInt::HttpWbxApiMaxRetryBudgetPerMinute);
		}
	}

	void HttpWbxApiService::newServiceAdded(shared_ptr<Instance> newService)
	{
		checkForClientAndServer(this);
	}

	void HttpWbxApiService::checkForClientAndServer(Instance* context)
	{
		if (!serverPresent && Workspace::serverIsPresent(context))
		{
			disconnectEventConnections();

			defaultServerThrottle.addBudget(DFInt::HttpWbxApiRequestsPerMinuteServerLimit, DFInt::HttpWbxApiRequestsPerMinuteServerLimit);
			elevatedServerThrottle.addBudget(DFInt::HttpWbxApiRequestsPerMinuteServerLimit, DFInt::HttpWbxApiRequestsPerMinuteServerLimit);
			retryBudget.addBudget(DFInt::HttpWbxApiMaxRetryBudgetPerMinute, DFInt::HttpWbxApiMaxRetryBudgetPerMinute);

			serverPresent = true;
		}

		if (!clientPresent && Workspace::clientIsPresent(context))
		{
			disconnectEventConnections();

			clientThrottle.addBudget(DFInt::HttpWbxApiClientPerMinuteRequestLimit, DFInt::HttpWbxApiClientPerMinuteRequestLimit);
			retryBudget.addBudget(DFInt::HttpWbxApiMaxRetryBudgetPerMinute, DFInt::HttpWbxApiMaxRetryBudgetPerMinute);

			clientPresent = true;
		}
	}

	void HttpWbxApiService::setErrorForAsync(const std::string& errorString, boost::function<void(std::string)> errorFunction)
	{
		if (errorFunction && !errorFunction.empty())
		{
			errorFunction(errorString);
		}
	}

	bool HttpWbxApiService::isAPIHttpRequest(const Http& httpRequest)
	{
		if (StaticApiBaseUrl.empty())
		{
			return false;
		}

		return (httpRequest.url.find(StaticApiBaseUrl) != std::string::npos);
	}

	std::string HttpWbxApiService::getApiUrlPath(const Http& httpRequest)
	{
		const std::string urlString = httpRequest.url;

		size_t pos = urlString.find(StaticApiBaseUrl);
		if (pos != std::string::npos)
		{
			return urlString.substr(pos + StaticApiBaseUrl.length(), urlString.length());
		}

		return "";
	}

	static bool shouldRetryFromStatusCode(const long statusCode, HttpWbxApiService::HttpApiRequest& request)
	{
		// retry on 503 error, this means the web service is throttling
		// web is switching to 429 for this, so also check 429
		if (statusCode == 503 ||
			statusCode == 429)
		{
			return true;
		}

		return false;
	}

	static bool shouldRetryRequest(std::exception* exception, HttpWbxApiService::HttpApiRequest& request, bool recordInGoogleAnalytics)
	{
		bool shouldRetry = false;
		if (exception)
		{		
			long statusCode = -1;
			if (wbx::http_status_error* httpError = dynamic_cast<wbx::http_status_error*>(exception))
			{
				statusCode = httpError->statusCode;
			}

			if (request.retryCount == 0 && recordInGoogleAnalytics)
			{
				WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "RetryRequestStarted");
			}

			shouldRetry = shouldRetryFromStatusCode(statusCode, request) && (request.retryCount < DFInt::HttpWbxApiMaxRetryCount);
			request.retryCount++;
		}

		return shouldRetry;
	}

	bool HttpWbxApiService::retrySyncRequest(Http& http, std::string& syncResponse)
	{
		int retryCount = 0;
		while (retryCount < DFInt::HttpWbxApiMaxSyncRetries)
		{
			retryCount++;
			boost::this_thread::sleep(boost::posix_time::milliseconds(DFInt::HttpWbxApiSyncRetryWaitTimeMSec));

			try
			{
				http.get(syncResponse,false);
				return true;
			}
			catch (std::exception&)
			{
				// just continue retrying, don't throw
			}
		}

		return false;
	}

	void httpHelperRetryLockAcquired(
		shared_ptr<HttpWbxApiService> apiService, 
		HttpWbxApiService::HttpApiRequest request,  
		boost::function<void(std::string)> errorFunction)
	{
		if (!apiService->addToRetryQueue(request))
		{
			errorFunction("Request failed, but could not retry due to too many current retry requests.");
			return;
		}
	}

	void httpHelperExecuteLockAcquired(
		shared_ptr<HttpWbxApiService> apiService, 
		shared_ptr<std::string> response, 
		shared_ptr<std::exception> exception, 
		HttpWbxApiService::HttpApiRequest request, 
		boost::function<void(std::string)> resumeFunction, 
		boost::function<void(std::string)> errorFunction)
	{
		if(response)
		{
			if (apiService->getRecordInGoogleAnalytics() && request.retryCount > 0)
			{
				WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "RetryRequestSucceeded");
			}

			resumeFunction(*response);
		}
		else
		{
			if (apiService->getRecordInGoogleAnalytics() && request.retryCount > 0)
			{
				WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", "RetryRequestFailed");
			}

			errorFunction(exception->what());
		}
	}

	void HttpWbxApiService::httpHelper(weak_ptr<HttpWbxApiService> weakApiService, std::string* response, std::exception* exception, HttpApiRequest request, ThrottlingPriority throttlePriority, boost::function<void(std::string)> resumeFunction, boost::function<void(std::string)> errorFunction)
	{ 
		if (shared_ptr<HttpWbxApiService> apiService = weakApiService.lock())
		{
			if (shouldRetryRequest(exception, request, apiService->getRecordInGoogleAnalytics())) // we got an error that requires retry, try to do it again
			{
				DataModel::processHttpRequestResponseOnLock(
					wbx::DataModel::get(apiService.get()),
					response,
					exception,
					boost::bind(&httpHelperRetryLockAcquired,
						apiService,
						request,
						errorFunction));
			}
			else
			{
				DataModel::processHttpRequestResponseOnLock(
					wbx::DataModel::get(apiService.get()),
					response,
					exception,
					boost::bind(&httpHelperExecuteLockAcquired,
						apiService,
						_1, /* shared_ptr<std::string> response, */
						_2, /* shared_ptr<std::exception> exception, */
						request,
						resumeFunction,
						errorFunction));
			}
		}
	}

	///////////////////////////////////////////////////////////////
	// HTTP Post Functions
	///////////////////////////////////////////////////////////////
	void HttpWbxApiService::postAsync(Http& httpRequest, std::string& data, ThrottlingPriority throttlePriority, HttpService::HttpContentType content,
		boost::function<void(std::string)> resumeFunction, boost::function<void(std::string)> errorFunction)
	{	
		if (!isAPIHttpRequest(httpRequest))
		{
			// don't pass non api calls here! Use regular http functions instead unless you want to get throttled
			WBXASSERT(false);
			if (getRecordInGoogleAnalytics())
			{
				WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", ("postAsyncNotApiRequest for " + httpRequest.url).c_str());
			}
		}

		postAsyncInternal(httpRequest, data, content, data.size() > HTTP_POST_COMPRESSION_LIMIT, throttlePriority, resumeFunction, errorFunction);
	}

    void HttpWbxApiService::checkAndUpdatePostUrl(std::string& fullUrl, const std::string& urlPath) const
    {
		if (urlPath.find(StaticApiBaseUrl) != std::string::npos)
		{
			// don't pass calls here with api proxy domain already in them!
			// just pass the path, ex: adimpression/validate-request
			WBXASSERT(false);

			if (getRecordInGoogleAnalytics())
			{
				WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", std::string("postAsyncUrlPathContainsDomain for " + urlPath).c_str());
			}

			fullUrl = urlPath;
		}
    }

	void HttpWbxApiService::postAsync(std::string urlPath, std::string data, bool useHttps, ThrottlingPriority throttlePriority, HttpService::HttpContentType content, 
		boost::function<void(std::string)> resumeFunction, boost::function<void(std::string)> errorFunction)
	{
		std::string fullUrl = "https" + apiBaseUrl + urlPath;
        checkAndUpdatePostUrl(fullUrl, urlPath);

		Http http(fullUrl);
		postAsyncInternal(http, data, content, data.size() > HTTP_POST_COMPRESSION_LIMIT, throttlePriority, resumeFunction, errorFunction);
	}

	void HttpWbxApiService::postAsyncWithAdditionalHeaders(std::string urlPath, std::string data,bool useHttps, ThrottlingPriority throttlePriority, HttpService::HttpContentType content, wbx::HttpAux::AdditionalHeaders additionalHeaders,
		boost::function<void(std::string)> resumeFunction, boost::function<void(std::string)> errorFunction)
	{
		std::string fullUrl = "https" + apiBaseUrl + urlPath;
        checkAndUpdatePostUrl(fullUrl, urlPath);

		Http http(fullUrl);
		if (additionalHeaders.size() > 0)
		{
			http.applyAdditionalHeaders(additionalHeaders);
		}
		postAsyncInternal(http, data, content, data.size() > HTTP_POST_COMPRESSION_LIMIT, throttlePriority, resumeFunction, errorFunction);
	}

	void HttpWbxApiService::postAsyncLua(std::string urlPath, std::string data, bool useHttps, ThrottlingPriority throttlePriority, HttpService::HttpContentType content, 
		boost::function<void(std::string)> resumeFunction, boost::function<void(std::string)> errorFunction)
	{
		std::string fullUrl = "https" + apiBaseUrl + urlPath;
		checkAndUpdatePostUrl(fullUrl, urlPath);

		Http http(fullUrl);
		WiibloxScriptModifiedCheck(wbx::apiPostAsyncFunction.security);
		postAsyncInternal(http, data, content, data.size() > HTTP_POST_COMPRESSION_LIMIT, throttlePriority, resumeFunction, errorFunction);
	}

	void HttpWbxApiService::postAsyncInternal(Http& httpRequest, std::string& data, const HttpService::HttpContentType& contentType, const bool shouldCompress, const ThrottlingPriority& throttlePriority,
		boost::function<void(std::string)> resumeFunction, boost::function<void(std::string)> errorFunction)
	{
		std::string contentTypeName;

		switch(contentType)
		{
		case HttpService::APPLICATION_JSON: contentTypeName = Http::kContentTypeApplicationJson; break;
		case HttpService::APPLICATION_XML: contentTypeName = Http::kContentTypeApplicationXml; break;
		case HttpService::APPLICATION_URLENCODED: contentTypeName = Http::kContentTypeUrlEncoded; break;
		case HttpService::TEXT_PLAIN: contentTypeName = Http::kContentTypeTextPlain; break;
		case HttpService::TEXT_XML: contentTypeName = Http::kContentTypeTextXml; break;
		default:
			errorFunction("Unsupported content type");
			return; 
		}

		if (data.empty())
		{
			data = " ";
		}

		HttpApiRequest apiRequest;
		apiRequest.isPost = true;
		apiRequest.postData = data;
		apiRequest.httpContentType = contentTypeName;
		apiRequest.resumeFunction = resumeFunction;
		apiRequest.errorFunction = errorFunction;
		apiRequest.throttlingPriority = throttlePriority;
		apiRequest.setHttp(httpRequest);

		executeApiRequest(apiRequest, throttlePriority, errorFunction);
	}

	///////////////////////////////////////////////////////////////
	// HTTP Get Functions
	///////////////////////////////////////////////////////////////
	
	void HttpWbxApiService::get(const std::string& urlPath, bool useHttps, ThrottlingPriority throttlePriority, std::string& response)
	{
		std::string fullUrl = "https" + apiBaseUrl + urlPath;

		Http http(fullUrl);

		get(http, useHttps, throttlePriority, response);
	}

	void HttpWbxApiService::get(Http& httpRequest, bool useHttps, ThrottlingPriority throttlePriority, std::string& response)
	{
		HttpApiRequest apiRequest;
		apiRequest.async = false;
		apiRequest.throttlingPriority = throttlePriority;
		apiRequest.setHttp(httpRequest);

		executeApiRequest(apiRequest, throttlePriority, NULL);

		response = apiRequest.syncResponse;
	}

	void HttpWbxApiService::getAsync(Http& httpRequest, ThrottlingPriority throttlePriority, boost::function<void(std::string)> resumeFunction, boost::function<void(std::string)> errorFunction)
	{
		if (!isAPIHttpRequest(httpRequest))
		{
			// don't pass non api calls here! Use regular http functions instead unless you want to get throttled
			WBXASSERT(false);

			if (getRecordInGoogleAnalytics())
			{
				WiibloxGoogleAnalytics::trackEvent(GA_CATEGORY_GAME, "HttpWbxApiService", std::string("getAsyncNonApiCall for " + httpRequest.url).c_str());
			}
		}

		getAsyncInternal(httpRequest, throttlePriority, resumeFunction, errorFunction);
	}

	void HttpWbxApiService::getAsync(std::string urlPath, bool useHttps, ThrottlingPriority throttlePriority, boost::function<void(std::string)> resumeFunction, boost::function<void(std::string)> errorFunction)
	{
		std::string fullUrl = "https" + apiBaseUrl + urlPath;

        Http http(fullUrl);
		getAsyncInternal(http, throttlePriority, resumeFunction, errorFunction);
	}

	void HttpWbxApiService::getAsyncLua(std::string urlPath, bool useHttps, ThrottlingPriority throttlePriority, boost::function<void(std::string)> resumeFunction, boost::function<void(std::string)> errorFunction)
	{
		std::string fullUrl = "https" + apiBaseUrl + urlPath;


		Http http(fullUrl);
		WiibloxScriptModifiedCheck(wbx::apiGetAsyncFunction.security);
		getAsyncInternal(http, throttlePriority, resumeFunction, errorFunction);
	}

	void HttpWbxApiService::getAsyncInternal(Http& httpRequest, const ThrottlingPriority& throttlePriority, boost::function<void(std::string)> resumeFunction, boost::function<void(std::string)> errorFunction)
	{
		HttpApiRequest apiRequest;
		apiRequest.throttlingPriority = throttlePriority;
		apiRequest.resumeFunction = resumeFunction;
		apiRequest.errorFunction = errorFunction;
		apiRequest.setHttp(httpRequest);

		executeApiRequest(apiRequest, throttlePriority, errorFunction);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////
	// Throttling Logic
	//////////////////////////////////////////////////////////////

	int HttpWbxApiService::getPlayerNum()
	{
		const ServiceProvider* serviceProvider = ServiceProvider::findServiceProvider(this);
		if (serviceProvider == NULL) 
		{
			WBXASSERT(false);
			return 0;
		}

		Network::Players* players = serviceProvider->find<Network::Players>();
		return players ? players->getNumPlayers() : 0;
	}

	void HttpWbxApiService::addThrottlingBudgets(float timeDeltaMinutes)
	{
		if (Network::Players::serverIsPresent(this) || isPlaySolo)
		{
			const int serverMaxBudget = (DFInt::HttpWbxApiRequestsPerMinutePerPlayerInServerLimit * getPlayerNum()) + DFInt::HttpWbxApiRequestsPerMinuteServerLimit;
			const float addedServerBudget = timeDeltaMinutes*serverMaxBudget;

			defaultServerThrottle.addBudget(addedServerBudget, serverMaxBudget);
			elevatedServerThrottle.addBudget(addedServerBudget, serverMaxBudget);

			FASTLOG3F(FLog::HttpWbxApiBudget, "Adding budget %f, default budget is at: %f, elevated budget is at: %f", 
				addedServerBudget, defaultServerThrottle.getBudget(), elevatedServerThrottle.getBudget());
		}

		if (Network::Players::clientIsPresent(this) || isPlaySolo)
		{
			const float addedClientBudget = timeDeltaMinutes * DFInt::HttpWbxApiClientPerMinuteRequestLimit;
			clientThrottle.addBudget(addedClientBudget, DFInt::HttpWbxApiMaxBudgetMultiplier *  DFInt::HttpWbxApiClientPerMinuteRequestLimit);

			FASTLOG2F(FLog::HttpWbxApiBudget, "Adding budget %f, client budget is at: %f", 
				addedClientBudget, clientThrottle.getBudget());
		}

		const int maxRetryBudget = DFInt::HttpWbxApiMaxRetryBudgetPerMinute;
		const float addedRetryBudget =  DFInt::HttpWbxApiMaxRetryBudgetPerMinute * timeDeltaMinutes;
		retryBudget.addBudget(addedRetryBudget, maxRetryBudget);
	}

	void HttpWbxApiService::HttpApiRequest::execute(HttpWbxApiService* apiService)
	{
		if (apiService->getRecordInGoogleAnalytics())
		{
			apiService->addToApiCallCount();
		}

		if (isPost)
		{
			http.post(postData, httpContentType,  (postData.size() > HTTP_POST_COMPRESSION_LIMIT), 
				boost::bind(&HttpWbxApiService::httpHelper, weak_from(apiService), _1, _2, *this, throttlingPriority, resumeFunction, errorFunction), false);
		}
		else // we are a get call
		{
			if (!async) // synchronous call to http (better be for good reason!)
			{
				try
				{
					http.get(syncResponse,false);
				}
				catch (std::exception& e)
				{
					// if synchronous call fails, we have to just retry here
					// still throw if we never get a proper response
					if (shouldRetryRequest(&e, *this, apiService->getRecordInGoogleAnalytics()))
					{
						if (!retrySyncRequest(http, syncResponse))
						{
                            throw std::runtime_error( format("Could not get a valid response for %s after retrying synchronous get.", http.url.c_str()).c_str() );
						}
					}
					else
					{
						throw e;
					}
				}
			}
			else
			{
				http.get(boost::bind(&HttpWbxApiService::httpHelper, weak_from(apiService), _1, _2, *this, throttlingPriority,  resumeFunction, errorFunction), false);
			}
		}
	}

	bool HttpWbxApiService::addToRetryQueue(HttpApiRequest apiRequest)
	{
		if (retryQueue.size() < (unsigned) DFInt::HttpWbxApiMaxRetryQueueSize)
		{
			retryQueue.push_back(apiRequest);
			return true;
		}

		return false;
	}

	void HttpWbxApiService::executeRetryRequests()
	{
		if (retryQueue.size() > 0)
		{
			FASTLOG2F(FLog::HttpWbxApiBudget, "Executing retry requests, size: %f, budget: %f", (float)retryQueue.size(), retryBudget.getBudget());
		}

		while(retryQueue.size() > 0 && retryBudget.checkAndReduceBudget())
		{
			HttpApiRequest request;
			retryQueue.pop_front(&request);

			executeApiRequest(request, request.throttlingPriority, request.errorFunction);
		}
	}

	void HttpWbxApiService::executeThrottledRequests(DoubleEndedVector<HttpApiRequest>& queue, BudgetedThrottlingHelper& helper)
	{
		if (queue.size() > 0)
		{
			FASTLOG2F(FLog::HttpWbxApiBudget, "Executing throttled requests, size: %f, budget: %f", (float)queue.size(), helper.getBudget());
		}

		while(queue.size() > 0 && helper.checkAndReduceBudget())
		{
			HttpApiRequest request;
			queue.pop_front(&request);
			request.execute(this);
		}
	}

	void HttpWbxApiService::executeThrottledRequests()
	{
		if (Network::Players::serverIsPresent(this) || isPlaySolo)
		{
			executeThrottledRequests(throttledDefaultServerRequests, defaultServerThrottle);
			executeThrottledRequests(throttledElevatedServerRequests, elevatedServerThrottle);
		}

		if (Network::Players::clientIsPresent(this) || isPlaySolo)
		{
			executeThrottledRequests(throttledClientRequests, clientThrottle);
		}
	}

	bool HttpWbxApiService::tryThrottleRequest(const HttpApiRequest& apiRequest, BudgetedThrottlingHelper& budgetThrottler, DoubleEndedVector<HttpApiRequest>& throttledRequestQueue,
		boost::function<void(std::string)> errorFunction)
	{
		if (!budgetThrottler.checkAndReduceBudget())
		{
			FASTLOG1F(FLog::HttpWbxApiBudget, "Throttling, budget: %f", budgetThrottler.getBudget());

			if (getRecordInGoogleAnalytics())
			{
				static boost::once_flag flag = BOOST_ONCE_INIT;
				boost::call_once(&sendApiServiceDidQueue, flag);
			}

			if (throttledRequestQueue.size() < (unsigned)DFInt::HttpWbxApiMaxThrottledQueueSize)
			{
				throttledRequestQueue.push_back(apiRequest);
			}
			else
			{
				if (getRecordInGoogleAnalytics())
				{
					static boost::once_flag flag = BOOST_ONCE_INIT;
					boost::call_once(&sendApiServiceDidThrottle, flag);
				}

				std::string throttleError = wbx::format("number of API requests/minute exceeded limit for HTTP API throttle. Please don't issue more than %i API requests/minute with server scripts and no more than %i API requests/minute with local scripts.",
					DFInt::HttpWbxApiRequestsPerMinuteServerLimit + (DFInt::HttpWbxApiRequestsPerMinutePerPlayerInServerLimit * getPlayerNum()), DFInt::HttpWbxApiClientPerMinuteRequestLimit);
				setErrorForAsync(throttleError, errorFunction);
			}

			return true;
		}

		return false;
	}

	bool HttpWbxApiService::executeApiRequest(HttpApiRequest& apiRequest, const ThrottlingPriority& throttlePriority, boost::function<void(std::string)> errorFunction)
	{
		if (apiRequest.getHttp().url.empty())
		{
			setErrorForAsync("Empty URL", errorFunction);
			return false;
		}

		if (apiRequest.getHttp().url.find(apiBaseUrl) == std::string::npos)
		{
			setErrorForAsync("Non-API Proxy BaseURL used. HttpWbxApiService is only for API Proxy calls.", errorFunction);
			return false;
		}

		switch (throttlePriority)
		{
		case PRIORITY_DEFAULT:
			{
				if (isPlaySolo &&
					tryThrottleRequest(apiRequest, defaultServerThrottle, throttledDefaultServerRequests, errorFunction))
				{
					return false;
				}
				else if ( (Network::Players::serverIsPresent(this) ) &&
					tryThrottleRequest(apiRequest, defaultServerThrottle, throttledDefaultServerRequests, errorFunction))
				{
					return false;
				}
				else if ( (Network::Players::clientIsPresent(this) ) &&
					tryThrottleRequest(apiRequest, clientThrottle, throttledClientRequests, errorFunction))
				{
					return false;
				}

				break;
			}
		case PRIORITY_SERVER_ELEVATED:
			{
				if (isPlaySolo &&
					tryThrottleRequest(apiRequest, elevatedServerThrottle, throttledElevatedServerRequests, errorFunction))
				{
					return false;
				}
				else if ( Network::Players::serverIsPresent(this) &&
					tryThrottleRequest(apiRequest, elevatedServerThrottle, throttledElevatedServerRequests, errorFunction))
				{
					return false;
				}
				else if ( Network::Players::clientIsPresent(this) &&
					tryThrottleRequest(apiRequest, clientThrottle, throttledClientRequests, errorFunction))
				{
					return false;
				}

				break;
			}
		case PRIORITY_EXTREME:
			{
				// don't do anything here, extreme priority request should always go thru
				break;
			}
		default:
			{
				setErrorForAsync("No priority set for API proxy request, please set a priority.", errorFunction);
				return false;
				break;
			}
		}

		apiRequest.execute(this);

		return true;
	}

} //namespace WBX
