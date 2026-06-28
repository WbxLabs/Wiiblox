#include "stdafx.h"

#include "V8DataModel/factoryregistration.h"

#include "FastLog.h"

#include "V8DataModel/Adornment.h"
#include "V8DataModel/BillboardGui.h"
#include "v8datamodel/SurfaceGui.h"
#include "V8DataModel/Accoutrement.h"
#include "V8DataModel/Backpack.h"
#include "V8DataModel/BadgeService.h"
#include "V8DataModel/BasicPartInstance.h"
#include "V8DataModel/BevelMesh.h"
#include "V8DataModel/BlockMesh.h"
#include "v8datamodel/CacheableContentProvider.h"
#include "V8DataModel/Camera.h"
#include "V8DataModel/ChangeHistory.h"
#include "V8DataModel/CharacterAppearance.h"
#include "V8DataModel/CharacterMesh.h"
#include "V8DataModel/ChatService.h"
#include "V8DataModel/ClickDetector.h"
#include "V8DataModel/ContentProvider.h"
#include "V8DataModel/Configuration.h"
#include "V8DataModel/CollectionService.h"
#include "V8DataModel/CSGDictionaryService.h"
#include "V8DataModel/CustomEvent.h"
#include "V8DataModel/CustomEventReceiver.h"
#include "V8DataModel/CylinderMesh.h"
#include "V8DataModel/VirtualUser.h"
#include "V8DataModel/LogService.h"
#include "V8DataModel/DataModelMesh.h"
#include "V8DataModel/DebrisService.h"
#include "V8DataModel/Decal.h"
#include "V8DataModel/DialogRoot.h"
#include "V8DataModel/DialogChoice.h"
#include "V8DataModel/DebugSettings.h"
#include "V8DataModel/PhysicsSettings.h"
#include "V8DataModel/ExtrudedPartInstance.h"
#include "V8DataModel/FriendService.h"
#include "V8DataModel/Folder.h"
#include "V8DataModel/RenderHooksService.h"
#include "V8DataModel/Test.h"
#include "V8DataModel/CookiesEngineService.h"
#include "V8DataModel/TeleportService.h"
#include "V8DataModel/PersonalServerService.h"
#include "V8DataModel/ScriptService.h"
#include "V8DataModel/UserInputService.h"
#include "v8datamodel/AssetService.h"
#include "v8datamodel/HttpService.h"
#include "v8datamodel/HttpWbxApiService.h"
#include "v8datamodel/DataStoreService.h"
#include "v8datamodel/TerrainRegion.h"
#include "v8datamodel/PathfindingService.h"
#include "v8datamodel/StarterPlayerService.h"
#include "v8datamodel/HandleAdornment.h"
#include "util/CellID.h"

#ifdef _PRISM_PYRAMID_
#include "V8DataModel/PrismInstance.h"
#include "V8DataModel/PyramidInstance.h"
#include "V8DataModel/ParallelRampInstance.h"
#include "V8DataModel/RightAngleRampInstance.h"
#include "V8DataModel/CornerWedgeInstance.h"
#endif

#include "V8DataModel/Explosion.h"
#include "V8DataModel/FaceInstance.h"
#include "V8DataModel/Feature.h"
#include "V8DataModel/FileMesh.h"
#include "V8DataModel/Fire.h"
#include "V8DataModel/Flag.h"
#include "V8DataModel/FlagStand.h"
#include "V8DataModel/FlyweightService.h"
#include "V8DataModel/ForceField.h"
#include "V8DataModel/GameSettings.h"
#include "V8DataModel/GameBasicSettings.h"
#include "Script/LuaSettings.h"
#include "Script/DebuggerManager.h"
#include "Script/ModuleScript.h"
#include "Script/LuaSourceContainer.h"
#include "V8DataModel/GeometryService.h"
#include "V8DataModel/GlobalSettings.h"
#include "V8DataModel/Gyro.h"
#include "V8DataModel/Handles.h"
#include "V8DataModel/HandlesBase.h"
#include "V8DataModel/ArcHandles.h"
#include "V8DataModel/Hopper.h"
#include "V8DataModel/JointInstance.h"
#include "V8DataModel/JointsService.h"
#include "V8DataModel/Lighting.h"
#include "V8DataModel/MeshContentProvider.h"
#include "V8DataModel/Message.h"
#include "V8DataModel/Mouse.h"
#include "V8DataModel/NonReplicatedCSGDictionaryService.h"
#include "V8DataModel/ParametricPartInstance.h"
#include "V8DataModel/PartInstance.h"
#include "V8DataModel/PhysicsService.h"
#include "V8DataModel/Platform.h"
#include "V8DataModel/PlayerGui.h"
#include "V8DataModel/PlayerScripts.h"
#include "V8DataModel/PlayerMouse.h"
#include "V8DataModel/PluginManager.h"
#include "V8DataModel/PluginMouse.h"
#include "V8DataModel/Seat.h"
#include "V8DataModel/SelectionBox.h"
#include "V8DataModel/SelectionSphere.h"
#include "V8dataModel/Sky.h"
#include "V8dataModel/SkateboardPlatform.h"
#include "V8DataModel/SkateboardController.h"
#include "V8DataModel/Smoke.h"
#include "v8datamodel/CustomParticleEmitter.h"
#include "V8DataModel/SolidModelContentProvider.h"
#include "V8DataModel/Sparkles.h"
#include "V8DataModel/SpawnLocation.h"
#include "V8Datamodel/SpecialMesh.h"
#include "V8DataModel/Stats.h"
#include "V8DataModel/SurfaceSelection.h"
#include "V8DataModel/Teams.h"
#include "V8DataModel/TextService.h"
#include "V8DataModel/TextureContentProvider.h"
#include "V8DataModel/TimerService.h"
#include "V8DataModel/Tool.h"
#include "V8DataModel/StudioTool.h"
#include "V8DataModel/TouchTransmitter.h"
#include "V8DataModel/usercontroller.h"
#include "V8DataModel/Value.h"
#include "V8DataModel/VehicleSeat.h"
#include "V8DataModel/Visit.h"
#include "V8DataModel/Workspace.h"
#include "V8DataModel/LocalWorkspace.h"
#include "Humanoid/Humanoid.h"
#include "Humanoid/StatusInstance.h"
#include "Humanoid/HumanoidState.h"
#include "script/scriptcontext.h"
#include "Script/Script.h"
#include "Script/CoreScript.h"
#include "V8DataModel/MarketplaceService.h"
#include "V8DataModel/GuiService.h"
#include "V8DataModel/GuiBase.h"
#include "V8DataModel/GuiBase3d.h"
#include "V8DataModel/TweenService.h"
#include "V8DataModel/GuiObject.h"
#include "V8DataModel/ScreenGui.h"
#include "V8DataModel/Frame.h"
#include "V8DataModel/Scale9Frame.h"
#include "V8DataModel/ImageButton.h"
#include "V8DataModel/ImageLabel.h"
#include "V8DataModel/TextButton.h"
#include "V8DataModel/TextLabel.h"
#include "V8DataModel/TextBox.h"
#include "V8DataModel/SelectionLasso.h"
#include "V8DataModel/TextureTrail.h"
#include "V8DataModel/FloorWire.h"
#include "V8DataModel/Animation.h"
#include "V8DataModel/AnimationController.h"
#include "V8DataModel/AnimationTrack.h"
#include "V8DataModel/AnimationTrackState.h"
#include "V8DataModel/Animator.h"
#include "V8DataModel/KeyframeSequenceProvider.h"
#include "V8DataModel/KeyframeSequence.h"
#include "V8DataModel/Keyframe.h"
#include "V8DataModel/Pose.h"
#include "V8DataModel/MegaCluster.h"
#include "V8DataModel/Bindable.h"
#include "V8DataModel/Light.h"
#include "V8DataModel/Remote.h"
#include "V8DataModel/PartOperation.h"
#include "V8DataModel/PartOperationAsset.h"
#include "V8DataModel/Attachment.h"
#include "v8datamodel/TouchInputService.h"
#include "v8datamodel/HapticService.h"

#include "util/CellID.h"

#include "network/NetworkPacketCache.h"
#include "network/NetworkClusterPacketCache.h"
#include "network/ChatFilter.h"

#include "util/Sound.h"
#include "util/SoundService.h"
#include "util/UDim.h"
#include "util/Faces.h"
#include "util/Axes.h"
#include "Util/ScriptInformationProvider.h"
#include "util/Action.h"
#include "util/Region3.h"
#include "util/KeywordFilter.h"
#include "util/SystemAddress.h"
#include "util/LuaWebService.h"
#include "util/wbxrandom.h"
#include "util/RunStateOwner.h"
#include "util/PhysicalProperties.h"
#include "V8DataModel/InsertService.h"
#include "V8DataModel/SocialService.h"
#include "V8DataModel/GamePassService.h"
#include "V8DataModel/ContextActionService.h"
#include "V8DataModel/LoginService.h"
#include "Util/ContentFilter.h"
#include "Tool/LuaDragger.h"
#include "Tool/AdvLuaDragger.h"
#include "WbxG3D/WbxTime.h"
#include "v8datamodel/InputObject.h"
#include "v8datamodel/ReplicatedStorage.h"
#include "v8datamodel/WiibloxReplicatedStorage.h"
#include "v8datamodel/ReplicatedFirst.h"
#include "v8datamodel/ServerScriptService.h"
#include "v8datamodel/ServerStorage.h"
#include "util/standardout.h"
#include "util/KeyCode.h"
#include "v8datamodel/PointsService.h"
#include "v8datamodel/ScrollingFrame.h"
#include "V8datamodel/AdService.h"
#include "V8datamodel/NotificationService.h"
#include "V8datamodel/GroupService.h"
#include "V8datamodel/GamepadService.h"

#include "v8datamodel/NumberSequence.h"
#include "v8datamodel/NumberRange.h"
#include "v8datamodel/ColorSequence.h"



using namespace WBX;

WBX_REGISTER_TYPE(void);
WBX_REGISTER_TYPE(bool);
WBX_REGISTER_TYPE(float);
WBX_REGISTER_TYPE(int);
WBX_REGISTER_TYPE(long);
WBX_REGISTER_TYPE(double);
WBX_REGISTER_TYPE(std::string);
WBX_REGISTER_TYPE(wbx::ProtectedString);
WBX_REGISTER_TYPE(const Reflection::PropertyDescriptor*);
WBX_REGISTER_TYPE(wbx::BrickColor);
WBX_REGISTER_TYPE(wbx::SystemAddress);
WBX_REGISTER_TYPE(wbx::MeshId);
WBX_REGISTER_TYPE(wbx::AnimationId);
WBX_REGISTER_TYPE(boost::shared_ptr<const Reflection::Tuple>);
WBX_REGISTER_TYPE(G3D::Vector3);
WBX_REGISTER_TYPE(G3D::Vector3int16);
WBX_REGISTER_TYPE(wbx::Region3);
WBX_REGISTER_TYPE(wbx::WBXRay);
WBX_REGISTER_TYPE(G3D::Rect2D);
WBX_REGISTER_TYPE(wbx::PhysicalProperties);
WBX_REGISTER_TYPE(wbx::Vector2);
WBX_REGISTER_TYPE(G3D::Vector2int16);
WBX_REGISTER_TYPE(G3D::Color3);
WBX_REGISTER_TYPE(G3D::CoordinateFrame);
WBX_REGISTER_TYPE(wbx::ContentId);
WBX_REGISTER_TYPE(wbx::TextureId);
WBX_REGISTER_TYPE(wbx::UDim);
WBX_REGISTER_TYPE(wbx::UDim2);
WBX_REGISTER_TYPE(wbx::Faces);
WBX_REGISTER_TYPE(wbx::Axes);
WBX_REGISTER_TYPE(boost::shared_ptr<const Instances>);
WBX_REGISTER_TYPE(boost::shared_ptr<class Reflection::DescribedBase>);
WBX_REGISTER_TYPE(boost::shared_ptr<class wbx::Instance>);
WBX_REGISTER_TYPE(Lua::WeakFunctionRef);
WBX_REGISTER_TYPE(shared_ptr<Lua::GenericFunction>);
WBX_REGISTER_TYPE(shared_ptr<Lua::GenericAsyncFunction>);
WBX_REGISTER_TYPE(boost::shared_ptr<const Reflection::ValueArray>);
WBX_REGISTER_TYPE(boost::shared_ptr<const Reflection::ValueMap>);
WBX_REGISTER_TYPE(boost::shared_ptr<const Reflection::ValueTable>);
WBX_REGISTER_TYPE(CellID);
WBX_REGISTER_TYPE(Soundscape::SoundId);
WBX_REGISTER_TYPE(wbx::NumberSequenceKeypoint);
WBX_REGISTER_TYPE(wbx::ColorSequenceKeypoint);
WBX_REGISTER_TYPE(wbx::NumberSequence);
WBX_REGISTER_TYPE(wbx::ColorSequence);
WBX_REGISTER_TYPE(wbx::NumberRange);
WBX_REGISTER_TYPE(wbx::Guid::Data);

WBX_REGISTER_CLASS(InputObject);
WBX_REGISTER_CLASS(TestService);
WBX_REGISTER_CLASS(FunctionalTest);
WBX_REGISTER_CLASS(Lighting);
WBX_REGISTER_CLASS(DebugSettings);
WBX_REGISTER_CLASS(PhysicsSettings);
WBX_REGISTER_CLASS(TaskSchedulerSettings);
WBX_REGISTER_CLASS(DataModel);
WBX_REGISTER_CLASS(PhysicsService);
WBX_REGISTER_CLASS(BadgeService);
WBX_REGISTER_CLASS(DialogRoot);
WBX_REGISTER_CLASS(DialogChoice);
WBX_REGISTER_CLASS(Tool);
WBX_REGISTER_CLASS(StudioTool);
WBX_REGISTER_CLASS(LuaDragger);
WBX_REGISTER_CLASS(AdvLuaDragger);
WBX_REGISTER_CLASS(Accoutrement);
WBX_REGISTER_CLASS(Backpack);
WBX_REGISTER_CLASS(BodyColors);
WBX_REGISTER_CLASS(ClickDetector);
WBX_REGISTER_CLASS(ControllerService);
WBX_REGISTER_CLASS(ChatService);
WBX_REGISTER_CLASS(TextService);
WBX_REGISTER_CLASS(VirtualUser);
WBX_REGISTER_CLASS(Explosion);
WBX_REGISTER_CLASS(Team);
WBX_REGISTER_CLASS(Instance);
WBX_REGISTER_CLASS(Flag);
WBX_REGISTER_CLASS(FlagStand);
WBX_REGISTER_CLASS(FlagStandService);
WBX_REGISTER_CLASS(ForceField);
WBX_REGISTER_CLASS(Fire);
WBX_REGISTER_CLASS(GameSettings);
WBX_REGISTER_CLASS(GameBasicSettings);
WBX_REGISTER_CLASS(GeometryService);
WBX_REGISTER_CLASS(Settings);
WBX_REGISTER_CLASS(GlobalAdvancedSettings);
WBX_REGISTER_CLASS(GlobalBasicSettings);
WBX_REGISTER_CLASS(Hat);
WBX_REGISTER_CLASS(Accessory);
WBX_REGISTER_CLASS(Hint);
WBX_REGISTER_CLASS(Humanoid);
WBX_REGISTER_CLASS(StatusInstance);
WBX_REGISTER_CLASS(RunService);
WBX_REGISTER_CLASS(LegacyHopperService);
WBX_REGISTER_CLASS(LocalScript);
WBX_REGISTER_CLASS(LocalWorkspace);
WBX_REGISTER_CLASS(LuaSettings);
WBX_REGISTER_CLASS(CoreScript);
WBX_REGISTER_CLASS(Message);
WBX_REGISTER_CLASS(Selection);
WBX_REGISTER_CLASS(ShirtGraphic);
WBX_REGISTER_CLASS(Shirt);
WBX_REGISTER_CLASS(Pants);
WBX_REGISTER_CLASS(Smoke);
WBX_REGISTER_CLASS(CustomParticleEmitter);
WBX_REGISTER_CLASS(Sparkles);
WBX_REGISTER_CLASS(StarterPackService);
WBX_REGISTER_CLASS(StarterPlayerService);
WBX_REGISTER_CLASS(StarterGuiService);
WBX_REGISTER_CLASS(UserInputService);
WBX_REGISTER_CLASS(CoreGuiService);
WBX_REGISTER_CLASS(StarterGear);
WBX_REGISTER_CLASS(Visit);
WBX_REGISTER_CLASS(ObjectValue);
WBX_REGISTER_CLASS(IntValue);
WBX_REGISTER_CLASS(DoubleValue);
WBX_REGISTER_CLASS(BoolValue);
WBX_REGISTER_CLASS(StringValue);
WBX_REGISTER_CLASS(BinaryStringValue);
WBX_REGISTER_CLASS(Vector3Value);
WBX_REGISTER_CLASS(RayValue);
WBX_REGISTER_CLASS(CFrameValue);
WBX_REGISTER_CLASS(Color3Value);
WBX_REGISTER_CLASS(BrickColorValue);
WBX_REGISTER_CLASS(IntConstrainedValue);
WBX_REGISTER_CLASS(DoubleConstrainedValue);
WBX_REGISTER_CLASS(Platform);
WBX_REGISTER_CLASS(SkateboardPlatform);
WBX_REGISTER_CLASS(Seat);
WBX_REGISTER_CLASS(VehicleSeat);
WBX_REGISTER_CLASS(DebrisService);
WBX_REGISTER_CLASS(TimerService);
WBX_REGISTER_CLASS(SpawnerService);
WBX_REGISTER_CLASS(ContentFilter);
WBX_REGISTER_CLASS(InsertService);
WBX_REGISTER_CLASS(LuaWebService);
WBX_REGISTER_CLASS(FriendService);
WBX_REGISTER_CLASS(RenderHooksService);
WBX_REGISTER_CLASS(CookiesService);
WBX_REGISTER_CLASS(SocialService);
WBX_REGISTER_CLASS(GamePassService);
WBX_REGISTER_CLASS(MarketplaceService);
WBX_REGISTER_CLASS(GroupService);
WBX_REGISTER_CLASS(ContextActionService);
WBX_REGISTER_CLASS(PersonalServerService);
WBX_REGISTER_CLASS(AssetService);
WBX_REGISTER_CLASS(ScriptService);
WBX_REGISTER_CLASS(ContentProvider);
WBX_REGISTER_CLASS(MeshContentProvider);
WBX_REGISTER_CLASS(TextureContentProvider);
WBX_REGISTER_CLASS(SolidModelContentProvider);
WBX_REGISTER_CLASS(CacheableContentProvider);
WBX_REGISTER_CLASS(ChangeHistoryService);
WBX_REGISTER_CLASS(HttpService);
WBX_REGISTER_CLASS(HttpWbxApiService);
WBX_REGISTER_CLASS(DataStoreService);
WBX_REGISTER_CLASS(PathfindingService);
WBX_REGISTER_CLASS(Path);
WBX_REGISTER_CLASS(Clothing);
WBX_REGISTER_CLASS(Skin);
WBX_REGISTER_CLASS(CharacterMesh);
WBX_REGISTER_CLASS(DataModelMesh);
WBX_REGISTER_CLASS(FileMesh);
WBX_REGISTER_CLASS(SpecialShape);
WBX_REGISTER_CLASS(BevelMesh);
WBX_REGISTER_CLASS(BlockMesh);
WBX_REGISTER_CLASS(CylinderMesh);
//WBX_REGISTER_CLASS(EggMesh);
WBX_REGISTER_CLASS(ServiceProvider);
WBX_REGISTER_CLASS(RootInstance);
WBX_REGISTER_CLASS(ModelInstance);
WBX_REGISTER_CLASS(BaseScript);
WBX_REGISTER_CLASS(Script);
WBX_REGISTER_CLASS(ScriptContext);
WBX_REGISTER_CLASS(RuntimeScriptService);
WBX_REGISTER_CLASS(ScriptInformationProvider);
WBX_REGISTER_CLASS(Workspace);
WBX_REGISTER_CLASS(Controller);
WBX_REGISTER_CLASS(HumanoidController);
WBX_REGISTER_CLASS(VehicleController);
WBX_REGISTER_CLASS(SkateboardController);
WBX_REGISTER_CLASS(Pose);
WBX_REGISTER_CLASS(Keyframe);
WBX_REGISTER_CLASS(KeyframeSequence);
WBX_REGISTER_CLASS(KeyframeSequenceProvider);
WBX_REGISTER_CLASS(Animation);
WBX_REGISTER_CLASS(AnimationController);
WBX_REGISTER_CLASS(AnimationTrack);
WBX_REGISTER_CLASS(AnimationTrackState);
WBX_REGISTER_CLASS(Animator);
WBX_REGISTER_CLASS(TeleportService);
WBX_REGISTER_CLASS(CharacterAppearance);
WBX_REGISTER_CLASS(LogService);
WBX_REGISTER_CLASS(ScrollingFrame);
WBX_REGISTER_CLASS(FlyweightService);
WBX_REGISTER_CLASS(CSGDictionaryService);
WBX_REGISTER_CLASS(NonReplicatedCSGDictionaryService);
WBX_REGISTER_CLASS(TouchInputService);


// network
WBX_REGISTER_CLASS(Network::PhysicsPacketCache);
WBX_REGISTER_CLASS(Network::InstancePacketCache);
WBX_REGISTER_CLASS(Network::ClusterPacketCache);
WBX_REGISTER_CLASS(Network::OneQuarterClusterPacketCache);
WBX_REGISTER_CLASS(Network::ChatFilter);

// Joints - in alpha order
WBX_REGISTER_CLASS(JointsService);
WBX_REGISTER_CLASS(Glue);
WBX_REGISTER_CLASS(Motor);
WBX_REGISTER_CLASS(Motor6D);
WBX_REGISTER_CLASS(Rotate);
WBX_REGISTER_CLASS(RotateP);
WBX_REGISTER_CLASS(RotateV);
WBX_REGISTER_CLASS(Snap);
WBX_REGISTER_CLASS(Weld);
WBX_REGISTER_CLASS(ManualSurfaceJointInstance);
WBX_REGISTER_CLASS(ManualWeld);
WBX_REGISTER_CLASS(ManualGlue);
WBX_REGISTER_CLASS(BodyMover);
WBX_REGISTER_CLASS(TouchTransmitter);
WBX_REGISTER_CLASS(FaceInstance);
WBX_REGISTER_CLASS(Sky);
WBX_REGISTER_CLASS(PVInstance);
WBX_REGISTER_CLASS(VelocityMotor);
WBX_REGISTER_CLASS(Feature);
WBX_REGISTER_CLASS(DynamicRotate);
WBX_REGISTER_CLASS(JointInstance);
WBX_REGISTER_CLASS(Attachment);
WBX_REGISTER_CLASS(SpawnLocation);
WBX_REGISTER_CLASS(Mouse);
WBX_REGISTER_CLASS(PlayerMouse);
WBX_REGISTER_CLASS(Teams);
WBX_REGISTER_CLASS(BackpackItem);
WBX_REGISTER_CLASS(HopperBin);
WBX_REGISTER_CLASS(Camera);
WBX_REGISTER_CLASS(BasePlayerGui);
WBX_REGISTER_CLASS(PlayerGui);
WBX_REGISTER_CLASS(PlayerScripts);
WBX_REGISTER_CLASS(StarterPlayerScripts);
WBX_REGISTER_CLASS(StarterCharacterScripts);
WBX_REGISTER_CLASS(PartInstance);
WBX_REGISTER_CLASS(FormFactorPart);
WBX_REGISTER_CLASS(BasicPartInstance);
WBX_REGISTER_CLASS(ExtrudedPartInstance);
WBX_REGISTER_CLASS(PART::Wedge);
WBX_REGISTER_CLASS(Decal);
WBX_REGISTER_CLASS(DecalTexture);
WBX_REGISTER_CLASS(TweenService);
WBX_REGISTER_CLASS(GuiItem);
WBX_REGISTER_CLASS(GuiBase);
WBX_REGISTER_CLASS(GuiBase2d);
WBX_REGISTER_CLASS(GuiBase3d);
WBX_REGISTER_CLASS(GuiRoot);
WBX_REGISTER_CLASS(GuiObject);
WBX_REGISTER_CLASS(GuiButton);
WBX_REGISTER_CLASS(GuiLabel);
WBX_REGISTER_CLASS(GuiMain);
WBX_REGISTER_CLASS(GuiLayerCollector);
WBX_REGISTER_CLASS(BillboardGui);
WBX_REGISTER_CLASS(ScreenGui);
WBX_REGISTER_CLASS(SurfaceGui);
WBX_REGISTER_CLASS(SelectionLasso);
WBX_REGISTER_CLASS(SelectionPartLasso);
WBX_REGISTER_CLASS(SelectionPointLasso);
WBX_REGISTER_CLASS(TextureTrail);
WBX_REGISTER_CLASS(FloorWire);
WBX_REGISTER_CLASS(GuiService);
WBX_REGISTER_CLASS(Frame);
WBX_REGISTER_CLASS(Scale9Frame);
WBX_REGISTER_CLASS(GuiImageButton);
WBX_REGISTER_CLASS(ImageLabel);
WBX_REGISTER_CLASS(GuiTextButton);
WBX_REGISTER_CLASS(TextBox);
WBX_REGISTER_CLASS(TextLabel);
WBX_REGISTER_CLASS(PartAdornment);
WBX_REGISTER_CLASS(PVAdornment);
WBX_REGISTER_CLASS(Handles);
WBX_REGISTER_CLASS(HandlesBase);
WBX_REGISTER_CLASS(ArcHandles);
WBX_REGISTER_CLASS(SelectionBox);
WBX_REGISTER_CLASS(SelectionSphere);
WBX_REGISTER_CLASS(HandleAdornment);
WBX_REGISTER_CLASS(BoxHandleAdornment);
WBX_REGISTER_CLASS(ConeHandleAdornment);
WBX_REGISTER_CLASS(CylinderHandleAdornment);
WBX_REGISTER_CLASS(SphereHandleAdornment);
WBX_REGISTER_CLASS(LineHandleAdornment);
WBX_REGISTER_CLASS(ImageHandleAdornment);
WBX_REGISTER_CLASS(SurfaceSelection);
WBX_REGISTER_CLASS(CollectionService);
WBX_REGISTER_CLASS(Configuration);
WBX_REGISTER_CLASS(Folder);
WBX_REGISTER_CLASS(MotorFeature);
WBX_REGISTER_CLASS(Hole);
WBX_REGISTER_CLASS(MegaClusterInstance);
WBX_REGISTER_CLASS(PluginMouse);
WBX_REGISTER_CLASS(PluginManager);
WBX_REGISTER_CLASS(Plugin);
WBX_REGISTER_CLASS(Toolbar);
WBX_REGISTER_CLASS(wbx::Button);
//Conditional parts here
#ifdef _PRISM_PYRAMID_
WBX_REGISTER_CLASS(PrismInstance);
WBX_REGISTER_CLASS(PyramidInstance);
WBX_REGISTER_CLASS(ParallelRampInstance);
WBX_REGISTER_CLASS(RightAngleRampInstance);
WBX_REGISTER_CLASS(CornerWedgeInstance);
#endif // _PRISM_PYRAMID_
WBX_REGISTER_CLASS(CustomEvent);
WBX_REGISTER_CLASS(CustomEventReceiver);
//WBX_REGISTER_CLASS(PropertyInstance);
WBX_REGISTER_CLASS(BindableFunction);
WBX_REGISTER_CLASS(BindableEvent);
WBX_REGISTER_CLASS(wbx::Scripting::DebuggerManager);
WBX_REGISTER_CLASS(wbx::Scripting::ScriptDebugger);
WBX_REGISTER_CLASS(wbx::Scripting::DebuggerBreakpoint);
WBX_REGISTER_CLASS(wbx::Scripting::DebuggerWatch);
WBX_REGISTER_CLASS(Light);
WBX_REGISTER_CLASS(PointLight);
WBX_REGISTER_CLASS(SpotLight);
WBX_REGISTER_CLASS(SurfaceLight);
WBX_REGISTER_CLASS(LoginService);
WBX_REGISTER_CLASS(ReplicatedStorage);
WBX_REGISTER_CLASS(WiibloxReplicatedStorage);
WBX_REGISTER_CLASS(ServerScriptService);
WBX_REGISTER_CLASS(ServerStorage);
WBX_REGISTER_CLASS(RemoteFunction);
WBX_REGISTER_CLASS(RemoteEvent);
WBX_REGISTER_CLASS(TerrainRegion);
WBX_REGISTER_CLASS(ModuleScript);
WBX_REGISTER_CLASS(PointsService);
WBX_REGISTER_CLASS(AdService);
WBX_REGISTER_CLASS(NotificationService);
WBX_REGISTER_CLASS(ReplicatedFirst);
WBX_REGISTER_CLASS(PartOperation);
WBX_REGISTER_CLASS(PartOperationAsset);
WBX_REGISTER_CLASS(UnionOperation);
WBX_REGISTER_CLASS(NegateOperation);
WBX_REGISTER_CLASS(Soundscape::SoundService);
WBX_REGISTER_CLASS(Soundscape::SoundChannel);
WBX_REGISTER_CLASS(GamepadService);
WBX_REGISTER_CLASS(LuaSourceContainer);
WBX_REGISTER_CLASS(HapticService);

// Xbox
#if defined(WBX_PLATFORM_DURANGO)
#include "v8datamodel/PlatformService.h"
WBX_REGISTER_CLASS(PlatformService);
#endif

static void onSlotException(std::exception& ex)
{
	FASTLOG(FLog::Error, "Slot Exception");
	wbx::StandardOut::singleton()->printf(MESSAGE_ERROR, "exception while signalling: %s", ex.what());
}

FactoryRegistrator::FactoryRegistrator()
{
	G3D::System::time();// Initialize the Program Start Time.
	registerSound();
	wbx::registerScriptDescriptors();
	registerBodyMovers();

	registerValueClasses();
	wbx::registerStatsClasses();
	wbx::Surface::registerSurfaceDescriptors();

	wbx::signals::slot_exception_handler = onSlotException;

	srand(wbx::randomSeed());

	ModelInstance::hackPhysicalCharacter();
}

// Enum types
WBX_REGISTER_ENUM(ChangeHistoryService::RuntimeUndoBehavior);
WBX_REGISTER_ENUM(FunctionalTest::Result);
WBX_REGISTER_ENUM(TaskScheduler::PriorityMethod);
WBX_REGISTER_ENUM(TaskScheduler::Job::SleepAdjustMethod);
WBX_REGISTER_ENUM(TaskScheduler::ThreadPoolConfig);
WBX_REGISTER_ENUM(Action::ActionType);
WBX_REGISTER_ENUM(Controller::Button);
WBX_REGISTER_ENUM(HopperBin::BinType);
WBX_REGISTER_ENUM(GuiObject::SizeConstraint);
WBX_REGISTER_ENUM(GuiObject::TweenEasingStyle);
WBX_REGISTER_ENUM(GuiObject::TweenStatus);
WBX_REGISTER_ENUM(GuiObject::TweenEasingDirection);
WBX_REGISTER_ENUM(TextService::XAlignment);
WBX_REGISTER_ENUM(TextService::YAlignment);
WBX_REGISTER_ENUM(TextService::FontSize);
WBX_REGISTER_ENUM(TextService::Font);
WBX_REGISTER_ENUM(Camera::CameraType);
WBX_REGISTER_ENUM(Camera::CameraMode);
WBX_REGISTER_ENUM(Camera::CameraPanMode);
WBX_REGISTER_ENUM(LegacyController::InputType);
WBX_REGISTER_ENUM(DataModelArbiter::ConcurrencyModel);
WBX_REGISTER_ENUM(DataModelMesh::LODType);
WBX_REGISTER_ENUM(DebugSettings::ErrorReporting);
WBX_REGISTER_ENUM(EThrottle::EThrottleType);
WBX_REGISTER_ENUM(Feature::InOut);
WBX_REGISTER_ENUM(Feature::LeftRight);
WBX_REGISTER_ENUM(Feature::TopBottom);
WBX_REGISTER_ENUM(Joint::JointType);
WBX_REGISTER_ENUM(KeywordFilterType);
WBX_REGISTER_ENUM(Legacy::SurfaceConstraint);
WBX_REGISTER_ENUM(NormalId);
WBX_REGISTER_ENUM(Vector3::Axis);
WBX_REGISTER_ENUM(Humanoid::Status);
WBX_REGISTER_ENUM(Humanoid::HumanoidRigType);
WBX_REGISTER_ENUM(Humanoid::NameOcclusion);
WBX_REGISTER_ENUM(Humanoid::HumanoidDisplayDistanceType);
WBX_REGISTER_ENUM(wbx::HUMAN::StateType);
WBX_REGISTER_ENUM(DataModel::CreatorType);
WBX_REGISTER_ENUM(DataModel::Genre);
WBX_REGISTER_ENUM(DataModel::GearGenreSetting);
WBX_REGISTER_ENUM(DataModel::GearType);
WBX_REGISTER_ENUM(Instance::SaveFilter);
WBX_REGISTER_ENUM(BasicPartInstance::LegacyPartType);
WBX_REGISTER_ENUM(KeyframeSequence::Priority);
WBX_REGISTER_ENUM(SocialService::StuffType);
WBX_REGISTER_ENUM(PersonalServerService::PrivilegeType);
WBX_REGISTER_ENUM(ExtrudedPartInstance::VisualTrussStyle);
#ifdef _PRISM_PYRAMID_
WBX_REGISTER_ENUM(PrismInstance::NumSidesEnum);
WBX_REGISTER_ENUM(PyramidInstance::NumSidesEnum);
#endif
WBX_REGISTER_ENUM(FriendService::FriendStatus);
WBX_REGISTER_ENUM(FriendService::FriendEventType);
WBX_REGISTER_ENUM(Handles::VisualStyle);
WBX_REGISTER_ENUM(SkateboardPlatform::MoveState);
WBX_REGISTER_ENUM(SoundType);
WBX_REGISTER_ENUM(SpecialShape::MeshType);
WBX_REGISTER_ENUM(SurfaceType);
WBX_REGISTER_ENUM(PartInstance::FormFactor);
WBX_REGISTER_ENUM(CollisionFidelity);
WBX_REGISTER_ENUM(UserInputService::SwipeDirection);
WBX_REGISTER_ENUM(UserInputService::Platform);
WBX_REGISTER_ENUM(UserInputService::MouseType);
WBX_REGISTER_ENUM(PartMaterial);
WBX_REGISTER_ENUM(PhysicalPropertiesMode);
WBX_REGISTER_ENUM(NetworkOwnership);
WBX_REGISTER_ENUM(Time::SampleMethod);
WBX_REGISTER_ENUM(GuiService::SpecialKey);
WBX_REGISTER_ENUM(GuiService::CenterDialogType);
WBX_REGISTER_ENUM(GuiService::UiMessageType);
WBX_REGISTER_ENUM(ChatService::ChatColor);
WBX_REGISTER_ENUM(MarketplaceService::CurrencyType);
WBX_REGISTER_ENUM(MarketplaceService::InfoType);
WBX_REGISTER_ENUM(CharacterMesh::BodyPart);
WBX_REGISTER_ENUM(GameSettings::VideoQuality);
WBX_REGISTER_ENUM(GameSettings::UploadSetting);
WBX_REGISTER_ENUM(GameBasicSettings::ControlMode);
WBX_REGISTER_ENUM(GameBasicSettings::RenderQualitySetting);
WBX_REGISTER_ENUM(GameBasicSettings::CameraMode);
WBX_REGISTER_ENUM(GameBasicSettings::TouchCameraMovementMode);
WBX_REGISTER_ENUM(GameBasicSettings::ComputerCameraMovementMode);
WBX_REGISTER_ENUM(GameBasicSettings::TouchMovementMode);
WBX_REGISTER_ENUM(GameBasicSettings::ComputerMovementMode);
WBX_REGISTER_ENUM(GameBasicSettings::RotationType);
WBX_REGISTER_ENUM(Frame::Style);
WBX_REGISTER_ENUM(GuiButton::Style);
WBX_REGISTER_ENUM(DialogRoot::DialogPurpose);
WBX_REGISTER_ENUM(DialogRoot::DialogTone);
WBX_REGISTER_ENUM(Voxel::CellMaterial);
WBX_REGISTER_ENUM(Voxel::CellBlock);
WBX_REGISTER_ENUM(Voxel::CellOrientation);
WBX_REGISTER_ENUM(Voxel::WaterCellForce);
WBX_REGISTER_ENUM(Voxel::WaterCellDirection);
WBX_REGISTER_ENUM(Explosion::ExplosionType);
WBX_REGISTER_ENUM(InputObject::UserInputType);
WBX_REGISTER_ENUM(InputObject::UserInputState);
WBX_REGISTER_ENUM(AssetService::AccessType);
WBX_REGISTER_ENUM(HttpService::HttpContentType);
WBX_REGISTER_ENUM(StarterGuiService::CoreGuiType);
WBX_REGISTER_ENUM(StarterPlayerService::DeveloperTouchCameraMovementMode);
WBX_REGISTER_ENUM(StarterPlayerService::DeveloperComputerCameraMovementMode);
WBX_REGISTER_ENUM(StarterPlayerService::DeveloperCameraOcclusionMode);
WBX_REGISTER_ENUM(StarterPlayerService::DeveloperTouchMovementMode);
WBX_REGISTER_ENUM(StarterPlayerService::DeveloperComputerMovementMode);
WBX_REGISTER_ENUM(TeleportService::TeleportState);
WBX_REGISTER_ENUM(TeleportService::TeleportType);
WBX_REGISTER_ENUM(KeyCode);
WBX_REGISTER_ENUM(MessageType);
WBX_REGISTER_ENUM(MarketplaceService::ProductPurchaseDecision);
WBX_REGISTER_ENUM(ThrottlingPriority);
WBX_REGISTER_ENUM(Soundscape::ReverbType);
WBX_REGISTER_ENUM(Soundscape::ListenerType);
WBX_REGISTER_ENUM(Soundscape::RollOffMode);
WBX_REGISTER_ENUM(PlayerActionType);
WBX_REGISTER_ENUM(RunService::RenderPriority);
WBX_REGISTER_ENUM(AdvArrowToolBase::JointCreationMode);
WBX_REGISTER_ENUM(GuiObject::ImageScale);
WBX_REGISTER_ENUM(UserInputService::OverrideMouseIconBehavior);
WBX_REGISTER_ENUM(Pose::PoseEasingStyle);
WBX_REGISTER_ENUM(Pose::PoseEasingDirection);
WBX_REGISTER_ENUM(HapticService::VibrationMotor);
WBX_REGISTER_ENUM(UserInputService::UserCFrame);

#if defined(WBX_PLATFORM_DURANGO)
WBX_REGISTER_ENUM(XboxKeyBoardType)
WBX_REGISTER_ENUM(VoiceChatState)
#endif
