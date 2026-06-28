#include "stdafx.h"

#include "Util/LegacyContentTable.h"
#include <boost/algorithm/string/predicate.hpp>

FASTFLAGVARIABLE(DebugRenderDownloadAssets, false)

namespace
{
	inline bool isSlash(char ch)
	{
		return ch == '\\' || ch == '/';
	}
	
	void normalizeUrl(std::string& url)
	{
		char* data = &url[0];
		size_t size = url.size();
		size_t write = 0;
		
		for (size_t i = 0; i < size; ++i)
		{
			// that's faster than tolower
			if (static_cast<unsigned int>(data[i] - 'A') < 26)
				data[write++] = (data[i] - 'A') + 'a';
			else if (isSlash(data[i]))
			{
				data[write++] = '/';
				
				// skip subsequent slashes
				while (i + 1 < size && isSlash(data[i + 1])) i++;
			}
			else
				data[write++] = data[i];
		}
		
		url.resize(write);
	}
}

namespace WBX
{
	LegacyContentTable::LegacyContentTable()
	{
		AddEntry("WBXasset://textures/A_Key_dn.png",				  "12222317");
		AddEntry("WBXasset://textures/A_Key.png",					  "12222308");
		AddEntry("WBXasset://textures/BaseballCapRed.png",			  "12222323");
		AddEntry("WBXasset://textures/bombtex.png",					  "12222348");
		AddEntry("WBXasset://textures/Bomb.png",					  "12222334");
		AddEntry("WBXasset://textures/CameraPanRight_ovr.png",		  "12222439");
		AddEntry("WBXasset://textures/CameraPanRight_dn.png",		  "12222430");
		AddEntry("WBXasset://textures/CameraPanRight.png",			  "12222420");
		AddEntry("WBXasset://textures/CameraPanLeft_ovr.png",		  "12222414");
		AddEntry("WBXasset://textures/CameraPanLeft_dn.png",		  "12222403");
		AddEntry("WBXasset://textures/CameraPanLeft.png",			  "12222388");
		AddEntry("WBXasset://textures/CameraCenter_ovr.png",		  "12222373");
		AddEntry("WBXasset://textures/CameraCenter_ds.png",			  "12222368");
		AddEntry("WBXasset://textures/CameraCenter_dn.png",			  "12222361");
		AddEntry("WBXasset://textures/CameraCenter.png",			  "12222351");
		AddEntry("WBXasset://textures/Character.png",				  "12222542");
		AddEntry("WBXasset://textures/CloneWandTexture.png",		  "12222613");
		AddEntry("WBXasset://textures/Controller1.png",				  "12222618");
		AddEntry("WBXasset://textures/Controller1_ds.png",			  "12222651");
		AddEntry("WBXasset://textures/Controller1_dn.png",			  "12222643");
		AddEntry("WBXasset://textures/Controller1_ovr.png",			  "12222658");
		AddEntry("WBXasset://textures/Controller1Tool_dn.png",		  "12222631");
		AddEntry("WBXasset://textures/Controller1Tool.png",			  "12222625");
		AddEntry("WBXasset://textures/Controller2_ovr.png",			  "12222710");
		AddEntry("WBXasset://textures/Controller2_ds.png",			  "12222700");
		AddEntry("WBXasset://textures/Controller2_dn.png",			  "12222687");
		AddEntry("WBXasset://textures/Controller2Tool_dn.png",		  "12222685");
		AddEntry("WBXasset://textures/Controller2Tool.png",			  "12222677");
		AddEntry("WBXasset://textures/Controller2.png",				  "12222671");
		AddEntry("WBXasset://textures/ControllerAI2Tool_dn.png",	  "12222736");
		AddEntry("WBXasset://textures/ControllerAI2Tool.png",		  "12222733");
		AddEntry("WBXasset://textures/ControllerAI1Tool_dn.png",	  "12222726");
		AddEntry("WBXasset://textures/ControllerAI1Tool.png",		  "12222720");
		AddEntry("WBXasset://textures/ControllerPanel.png",			  "12222767");
		AddEntry("WBXasset://textures/ControllerNoneTool_dn.png",	  "12222755");
		AddEntry("WBXasset://textures/ControllerNoneTool.png",		  "12222742");
		AddEntry("WBXasset://textures/D_Key_dn.png",				  "12222831");
		AddEntry("WBXasset://textures/D_Key.png",					  "12222825");
		AddEntry("WBXasset://textures/Down_key_dn.png",				  "12222788");
		AddEntry("WBXasset://textures/Down_key.png",				  "12222782");
		AddEntry("WBXasset://textures/Detonator.png",				  "12222773");
		AddEntry("WBXasset://textures/DropperTool_dn.png",			  "12222819");
		AddEntry("WBXasset://textures/DropperTool.png",				  "12222808");
		AddEntry("WBXasset://textures/DropperCursor.png",			  "12222805");
		AddEntry("WBXasset://textures/FireWand.png",				  "12222892");
		AddEntry("WBXasset://textures/FillTool_dn.png",				  "12222881");
		AddEntry("WBXasset://textures/FillTool.png",				  "12222872");
		AddEntry("WBXasset://textures/FlatTool_dn.png",				  "12223363");
		AddEntry("WBXasset://textures/FlatTool.png",				  "12223332");
		AddEntry("WBXasset://textures/Flamethrower.png",			  "12222937");
		AddEntry("WBXasset://textures/Gun.png",						  "12223438");
		AddEntry("WBXasset://textures/GlueCursor.png",				  "12223406");
		AddEntry("WBXasset://textures/Glue.png",					  "12223401");
		AddEntry("WBXasset://textures/HalloweenSkull.png",			  "12223869");
		AddEntry("WBXasset://textures/HalloweenRocket.png",			  "12223859");
		AddEntry("WBXasset://textures/HalloweenPumpkin.png",		  "12223503");
		AddEntry("WBXasset://textures/HalloweenGhost.png",			  "12223487");
		AddEntry("WBXasset://textures/HalloweenAsylum.png",			  "12223472");
		AddEntry("WBXasset://textures/Hammer.png",					  "12223874");
		AddEntry("WBXasset://textures/hammertex128.png",			  "12223896");
		AddEntry("WBXasset://textures/HopperPanel.png",				  "12229455");
		AddEntry("WBXasset://textures/H_Key_dn.png",				  "12223925");
		AddEntry("WBXasset://textures/H_Key.png",					  "12223920");
		AddEntry("WBXasset://textures/J_Key_dn.png",				  "12223934");
		AddEntry("WBXasset://textures/J_Key.png",					  "12229462");
		AddEntry("WBXasset://textures/K_Key_dn.png",				  "12223944");
		AddEntry("WBXasset://textures/K_Key.png",					  "12223938");
		AddEntry("WBXasset://textures/Laser.png",					  "12223948");
		AddEntry("WBXasset://textures/Left_key_dn.png",				  "12223985");
		AddEntry("WBXasset://textures/Left_key.png",				  "12223979");
		AddEntry("WBXasset://textures/LeftMotorTool_dn.png",		  "12223977");
		AddEntry("WBXasset://textures/LeftMotorTool.png",			  "12223968");
		AddEntry("WBXasset://textures/LeftMotorFastTool_dn.png",	  "12223964");
		AddEntry("WBXasset://textures/LeftMotorFastTool.png",		  "12223954");
		AddEntry("WBXasset://textures/MedKit.png",					  "12223996");
		AddEntry("WBXasset://textures/Multirocket.png",				  "12224010");
		AddEntry("WBXasset://textures/MultiSelection.png",			  "12224012");
		AddEntry("WBXasset://textures/PaintballGunTex128.png",		  "12224033");
		AddEntry("WBXasset://textures/PaintballIcon.png",			  "12229471");
		AddEntry("WBXasset://textures/pirate.png",					  "12229478");
		AddEntry("WBXasset://textures/PoliceCap.png",				  "12224039");
		AddEntry("WBXasset://textures/Right_key_dn.png",			  "12224062");
		AddEntry("WBXasset://textures/Right_key.png",				  "12224057");
		AddEntry("WBXasset://textures/Rocket.png",					  "17598704");
		AddEntry("WBXasset://textures/RocketBoots.png",				  "11900767");
		AddEntry("WBXasset://textures/rocketlaunchertex.png",		  "12224070");
		AddEntry("WBXasset://textures/S_Key_dn.png",				  "12224227");
		AddEntry("WBXasset://textures/S_Key.png",					  "12224224");
		AddEntry("WBXasset://textures/SlingshotTexture.png",		  "12224131");
		AddEntry("WBXasset://textures/Slingshot.png",				  "12224122");
		AddEntry("WBXasset://textures/Snowball.png",				  "12224133");
		AddEntry("WBXasset://textures/Snowflake.png",				  "12224137");
		AddEntry("WBXasset://textures/sombrero.png",				  "12224141");
		AddEntry("WBXasset://textures/Superball.png",				  "12224172");
		AddEntry("WBXasset://textures/SurfacePanel.png",			  "12224175");
		AddEntry("WBXasset://textures/SwordTexture.png",			  "12224218");
		AddEntry("WBXasset://textures/Sword128.png",				  "12224215");
		AddEntry("WBXasset://textures/test_texture.JPG",			  "12224240");
		AddEntry("WBXasset://textures/test2_texture.JPG",			  "12224234");
		AddEntry("WBXasset://textures/TopHatPurple.png",			  "12229488");
		AddEntry("WBXasset://textures/treetex.png",					  "12224246");
		AddEntry("WBXasset://textures/TrowelTexture.png",			  "12229499");
		AddEntry("WBXasset://textures/U_Key_dn.png",				  "12224281");
		AddEntry("WBXasset://textures/U_Key.png",					  "12224274");
		AddEntry("WBXasset://textures/Up_key_dn.png",				  "12224267");
		AddEntry("WBXasset://textures/Up_key.png",					  "12224262");
		AddEntry("WBXasset://textures/VelocityTool_ovr.png",		  "12224309");
		AddEntry("WBXasset://textures/VelocityTool_dn.png",			  "12224289");
		AddEntry("WBXasset://textures/VelocityTool.png",			  "12224285");
		AddEntry("WBXasset://textures/VelocityTool_ds.png",			  "12229500");
		AddEntry("WBXasset://textures/viking.png",					  "12224312");
		AddEntry("WBXasset://textures/W_Key_dn.png",                  "12224353");
		AddEntry("WBXasset://textures/W_Key.png",					  "12224347");
		AddEntry("WBXasset://textures/Wall.png",					  "12224320");
		
		AddEntry("WBXasset://sounds/Rocket whoosh 01.wav",			  "12222095");
		AddEntry("WBXasset://sounds/paintball.wav",					  "11900833");
		AddEntry("WBXasset://sounds/Short spring sound.wav",		  "12222124");
		AddEntry("WBXasset://sounds/Rocket shot.wav",				  "12222084");
		AddEntry("WBXasset://sounds/glassbreak.wav",				  "12222005");
		AddEntry("WBXasset://sounds/Launching rocket.wav",			  "12222065");
		AddEntry("WBXasset://sounds/Shoulder fired rocket.wav",		  "12222132");
		AddEntry("WBXasset://sounds/HalloweenThunder.wav",			  "12222030");
		AddEntry("WBXasset://sounds/HalloweenLightning.wav",		  "12222019");
		AddEntry("WBXasset://sounds/HalloweenGhost.wav",			  "12229501");
		


		AddEntry("WBXasset://icons/surface_ds.png",		"23576067");
		AddEntry("WBXasset://icons/surface.png",		"23576066");
		AddEntry("WBXasset://icons/rotate.png",			"23576065");
		AddEntry("WBXasset://icons/color_sel.png",		"23576064");
		AddEntry("WBXasset://icons/color.png",			"23576062");
		AddEntry("WBXasset://icons/insert.png",			"23576057");
		AddEntry("WBXasset://icons/freemove_ovr.png",	"23576055");	
		AddEntry("WBXasset://icons/surface_sel.png",	"23575961");	
		AddEntry("WBXasset://icons/surface_ovr.png",	"23575960");	
		AddEntry("WBXasset://icons/rotate_ovr.png",		"23575949");
		AddEntry("WBXasset://icons/rotate_ds.png",		"23575946");
		AddEntry("WBXasset://icons/resize_sel.png",		"23575942");
		AddEntry("WBXasset://icons/resize_ovr.png",		"23575941");
		AddEntry("WBXasset://icons/resize_ds.png",		"23575940");
		AddEntry("WBXasset://icons/resize.png",			"23575937");
		AddEntry("WBXasset://icons/color_ovr.png",		"23575933");
		AddEntry("WBXasset://icons/color_ds.png",		"23575928");
		AddEntry("WBXasset://icons/material_sel.png",	"23575919");	
		AddEntry("WBXasset://icons/material_ovr.png",	"23575918");	
		AddEntry("WBXasset://icons/material_ds.png",	"23575917");	
		AddEntry("WBXasset://icons/material.png",		"23575916");
		AddEntry("WBXasset://icons/lock_sel.png",		"23575914");
		AddEntry("WBXasset://icons/lock_ovr.png",		"23575912");
		AddEntry("WBXasset://icons/lock_ds.png",		"23575909");
		AddEntry("WBXasset://icons/lock.png",			"23575908");
		AddEntry("WBXasset://icons/insert_sel.png",		"23575907");
		AddEntry("WBXasset://icons/insert_ovr.png",		"23575906");
		AddEntry("WBXasset://icons/insert_ds.png",		"23575905");
		AddEntry("WBXasset://icons/freemove_sel.png",	"23575901");	
		AddEntry("WBXasset://icons/freemove_ds.png",	"23575889");	
		AddEntry("WBXasset://icons/freemove.png",		"23575887");
		AddEntry("WBXasset://icons/delete_sel.png",		"23575885");
		AddEntry("WBXasset://icons/delete_ovr.png",		"23575884");
		AddEntry("WBXasset://icons/delete_ds.png",		"23575883");
		AddEntry("WBXasset://icons/delete.png",			"23575880");
		AddEntry("WBXasset://icons/configure_sel.png",	"23575879");	
		AddEntry("WBXasset://icons/configure_ovr.png",	"23575878");	
		AddEntry("WBXasset://icons/configure_ds.png",	"23575877");	
		AddEntry("WBXasset://icons/configure.png",		"23575875");
		AddEntry("WBXasset://icons/axismove_sel.png",	"23575874");	
		AddEntry("WBXasset://icons/axismove_ovr.png",	"23575873");	
		AddEntry("WBXasset://icons/axismove_ds.png",	"23575872");	
		AddEntry("WBXasset://icons/axismove.png",		"23575871");
		AddEntry("WBXasset://icons/anchor_sel.png",		"23575870");
		AddEntry("WBXasset://icons/anchor_ovr.png",		"23575869");
		AddEntry("WBXasset://icons/anchor_ds.png",		"23575867");
		AddEntry("WBXasset://icons/anchor.png",			"23575866");

		AddEntry("WBXasset://icons/surface_dn.png",		"23629198"); 
		AddEntry("WBXasset://icons/rotate_dn.png",		"23629197"); 
		AddEntry("WBXasset://icons/resize_dn.png",		"23629194"); 
		AddEntry("WBXasset://icons/color_dn.png",		"23629193"); 
		AddEntry("WBXasset://icons/material_dn.png",	"23629192"); 
		AddEntry("WBXasset://icons/lock_dn.png",		"23629189"); 
		AddEntry("WBXasset://icons/insert_dn.png",		"23629186"); 
		AddEntry("WBXasset://icons/freemove_dn.png",	"23629182"); 
		AddEntry("WBXasset://icons/delete_dn.png",		"23629179"); 
		AddEntry("WBXasset://icons/configure_dn.png",	"23629177"); 
		AddEntry("WBXasset://icons/axismove_dn.png",	"23629176"); 
		AddEntry("WBXasset://icons/anchor_dn.png",		"23629212"); 
																
																 
	/*	AddEntry("WBXasset://textures/wrench.png",					  "12224344");
		//AddEntry("WBXasset://textures/WoodFallback.png",			  "12224336");
		AddEntry("WBXasset://textures/WeldCursor.png",				  "12224328");
		AddEntry("WBXasset://textures/UnlockCursor.png",			  "12224257");
		AddEntry("WBXasset://textures/UnAnchorCursor.png",			  "12224251");
		//AddEntry("WBXasset://textures/surfacesTrussAlpha.png",		  "12224211");
		//AddEntry("WBXasset://textures/SurfacesTruss.png",			  "12224200");
		//AddEntry("WBXasset://textures/SurfacesStrip.png",			  "12224195");
		//AddEntry("WBXasset://textures/surfacesAlpha.png",			  "12224189");
		//AddEntry("WBXasset://textures/Surfaces.png",				  "12224182");
		AddEntry("WBXasset://textures/SpawnLocation.png",			  "12224170");
		AddEntry("WBXasset://textures/SpawnCursor.png",				  "12224165");
		AddEntry("WBXasset://textures/sparkle.png",					  "12224156");
		//AddEntry("WBXasset://textures/SlateTileMono.png",			  "12224115");
		AddEntry("WBXasset://textures/script.png",					  "12224075");
		AddEntry("WBXasset://textures/ResizeCursor.png",			  "12224056");
		AddEntry("WBXasset://textures/ReportAbuse_ovr.PNG",			  "12224053");
		AddEntry("WBXasset://textures/ReportAbuse_ds.PNG",			  "12224050");
		AddEntry("WBXasset://textures/ReportAbuse_dn.PNG",			  "12224043");
		AddEntry("WBXasset://textures/ReportAbuse.PNG",				  "12224041");
		//AddEntry("WBXasset://textures/nosurfacesTrussAlpha.png",	  "12224030");
		//AddEntry("WBXasset://textures/nosurfacesTruss.png",			  "12224027");
		//AddEntry("WBXasset://textures/nosurfacesAlpha.png",			  "12224021");
		AddEntry("WBXasset://textures/MotorCursor.png",				  "12224008");
		AddEntry("WBXasset://textures/MissingCursor.png",			  "12224003");
		AddEntry("WBXasset://textures/LockCursor.png",				  "12223995");
		AddEntry("WBXasset://textures/HingeCursor.png",				  "12223901");
		AddEntry("WBXasset://textures/HammerOverCursor.png",		  "12223888");
		AddEntry("WBXasset://textures/HammerDownCursor.png",		  "12223885");
		AddEntry("WBXasset://textures/HammerCursor.png",			  "12223877");
		AddEntry("WBXasset://textures/GunWaitCursor.png",			  "12223457");
		AddEntry("WBXasset://textures/GunCursor.png",				  "12223447");
		AddEntry("WBXasset://textures/GrabRotateCursor.png",		  "12223431");
		AddEntry("WBXasset://textures/GrabCursor.png",				  "12223420");
		AddEntry("WBXasset://textures/Grab.png",					  "12223412");
		AddEntry("WBXasset://textures/GameTool.png",				  "12223382");
		AddEntry("WBXasset://textures/FlagCursor.png",				  "12222925");
		AddEntry("WBXasset://textures/FirstPersonIndicator_ds.png",	  "12222916");
		AddEntry("WBXasset://textures/FirstPersonIndicator.png",	  "12222902");
		AddEntry("WBXasset://textures/FillCursor.png",				  "12222858");
		AddEntry("WBXasset://textures/face.png",					  "12222850");
		AddEntry("WBXasset://textures/explosion.png",				  "12222842");
		AddEntry("WBXasset://textures/DragCursor.png",				  "12222794");
		
		AddEntry("WBXasset://textures/CloneOverCursor.png",			  "12222610");
		AddEntry("WBXasset://textures/CloneDownCursor.png",			  "12222606");
		AddEntry("WBXasset://textures/CloneCursor.png",				  "12222598");
		AddEntry("WBXasset://textures/Clone.png",					  "12222594");
		AddEntry("WBXasset://textures/Chat_ovr.png",				  "12222584");
		AddEntry("WBXasset://textures/Chat_ds.png",					  "12222575");
		AddEntry("WBXasset://textures/Chat_dn.png",					  "12222559");
		AddEntry("WBXasset://textures/Chat.png",					  "12222549");
		AddEntry("WBXasset://textures/CameraZoomOut_ovr.png",		  "12222532");
		AddEntry("WBXasset://textures/CameraZoomOut_dn.png",		  "12222524");
		AddEntry("WBXasset://textures/CameraZoomOut.png",			  "12222517");
		AddEntry("WBXasset://textures/CameraZoomIn_ovr.png",		  "12222510");
		AddEntry("WBXasset://textures/CameraZoomIn_dn.png",			  "12222505");
		AddEntry("WBXasset://textures/CameraZoomIn.png",			  "12222491");
		AddEntry("WBXasset://textures/CameraTiltUp_ovr.png",		  "12222484");
		AddEntry("WBXasset://textures/CameraTiltUp_dn.png",			  "12222475");
		AddEntry("WBXasset://textures/CameraTiltUp.png",			  "12222469");
		AddEntry("WBXasset://textures/CameraTiltDown_ovr.png",		  "12222460");
		AddEntry("WBXasset://textures/CameraTiltDown_dn.png",		  "12222454");
		AddEntry("WBXasset://textures/CameraTiltDown.png",			  "12222444");
		AddEntry("WBXasset://textures/Blank.png",					  "12222332");
		AddEntry("WBXasset://textures/ArrowFarCursor.png",			  "12222299");
		AddEntry("WBXasset://textures/ArrowCursorDecalDrag.png",	  "12222286");
		AddEntry("WBXasset://textures/ArrowCursor.png",				  "12222273");
		AddEntry("WBXasset://textures/AnchorCursor.png",			  "12222263");
		AddEntry("WBXasset://textures/FlatCursor.png",				  "12229442");
*/
		//AddEntry("WBXasset://textures/SlateTileMono.dds",			  "12224106");
		//AddEntry("WBXasset://textures/SlateTile.dds",				  "12224095");
		//AddEntry("WBXasset://textures/SlateBorder.dds",			  "12224079");
		//AddEntry("WBXasset://textures/noiseL8_32x32x32.dds",		  "12224017");
		//AddEntry("WBXasset://textures/HorizLines_normal.dds",		  "12223910");
/*
		AddEntry("WBXasset://sounds/victory.wav",                     "12222253");
		AddEntry("WBXasset://sounds/uuhhh.wav",						  "12222242");
		AddEntry("WBXasset://sounds/unsheath.wav",					  "12222225");
		AddEntry("WBXasset://sounds/swordslash.wav",				  "12222216");
		AddEntry("WBXasset://sounds/swordlunge.wav",				  "12222208");
		AddEntry("WBXasset://sounds/swoosh.wav",					  "12222200");
		AddEntry("WBXasset://sounds/SWITCH3.wav",					  "12222183");
		AddEntry("WBXasset://sounds/switch.wav",					  "12222170");
		AddEntry("WBXasset://sounds/splat.wav",						  "12222152");
		AddEntry("WBXasset://sounds/snap.wav",						  "12222140");
		
		AddEntry("WBXasset://sounds/Rubber band.wav",				  "12222106");
		AddEntry("WBXasset://sounds/Rubber band sling shot.wav",	  "12222103");
		
		AddEntry("WBXasset://sounds/pageturn.wav",					  "12222076");
		
		AddEntry("WBXasset://sounds/Kid saying Ouch.wav",			  "12222058");
		AddEntry("WBXasset://sounds/Kerplunk.wav",					  "12222054");
		//AddEntry("WBXasset://sounds/hit.wav",						  "12222046");*/
		
		AddEntry("WBXasset://fonts/BaseballCap.mesh",				  "12220916");
		AddEntry("WBXasset://fonts/clonewand.mesh",					  "12221344");
		AddEntry("WBXasset://fonts/fusedgirl.mesh",					  "12221423");
		AddEntry("WBXasset://fonts/girlhair.mesh",					  "12221431");
		AddEntry("WBXasset://fonts/hammer.mesh",					  "12221451");
		AddEntry("WBXasset://fonts/NinjaMask.mesh",					  "12221524");
		AddEntry("WBXasset://fonts/paintballgun.mesh",				  "11900867");
		AddEntry("WBXasset://fonts/pawn.mesh",						  "12221585");
		AddEntry("WBXasset://fonts/PirateHat.mesh",					  "12221595");
		AddEntry("WBXasset://fonts/PoliceCap.mesh",					  "12221603");
		AddEntry("WBXasset://fonts/rocketlauncher.mesh",			  "12221651");
		AddEntry("WBXasset://fonts/slingshot.mesh",					  "12221682");
		AddEntry("WBXasset://fonts/sombrero.mesh",					  "12221705");
		AddEntry("WBXasset://fonts/sword.mesh",						  "12221720");
		AddEntry("WBXasset://fonts/timebomb.mesh",					  "12221733");
		AddEntry("WBXasset://fonts/tophat.mesh",					  "12221750");
		AddEntry("WBXasset://fonts/tree.mesh",						  "12221787");
		AddEntry("WBXasset://fonts/trowel.mesh",					  "12221793");
		AddEntry("WBXasset://fonts/VikingHelmet.mesh",                "12221815");
		

	/*	
		AddEntry("WBXasset://sounds/flashbulb.wav",					  "12221996");
		AddEntry("WBXasset://sounds/electronicpingshort.wav",		  "12221990");
		AddEntry("WBXasset://sounds/collide.wav",					  "12221984");
		AddEntry("WBXasset://sounds/clickfast.wav",					  "12221976");
		AddEntry("WBXasset://sounds/button.wav",					  "12221967");
		AddEntry("WBXasset://sounds/bfsl-minifigfoots2.mp3",		  "12221962");
		AddEntry("WBXasset://sounds/bfsl-minifigfoots1.mp3",		  "12221952");
		AddEntry("WBXasset://sounds/bass.wav",						  "12221944");
		
		AddEntry("WBXasset://music/ufofly.wav",						  "12221842");
		//AddEntry("WBXasset://music/bass.wav",						  "12221831");

		//AddEntry("WBXasset://sky/sun.jpg",							  "12221937");				
		//AddEntry("WBXasset://sky/sun-rays.jpg",						  "12221930");
		//AddEntry("WBXasset://sky/skyspheremap.jpg",					  "12221923");
		//AddEntry("WBXasset://sky/null_plainsky512_up.jpg",			  "12221917");
		//AddEntry("WBXasset://sky/null_plainsky512_rt.jpg",			  "12221908");
		//AddEntry("WBXasset://sky/null_plainsky512_lf.jpg",			  "12221895");
		//AddEntry("WBXasset://sky/null_plainsky512_ft.jpg",			  "12221889");
		//AddEntry("WBXasset://sky/null_plainsky512_dn.jpg",			  "12221876");
		//AddEntry("WBXasset://sky/null_plainsky512_bk.jpg",			  "12221870");
		//AddEntry("WBXasset://sky/moon-alpha.jpg",					  "12221861");
		//AddEntry("WBXasset://sky/lensflare.jpg",					  "12221853");
		
		
		AddEntry("WBXasset://fonts/torso.mesh",						  "12221758");
		AddEntry("WBXasset://fonts/rightleg.mesh",					  "12221626");
		AddEntry("WBXasset://fonts/rightarm.mesh",					  "12221614");
		AddEntry("WBXasset://fonts/leftleg.mesh",					  "12221512");
		AddEntry("WBXasset://fonts/leftarm.mesh",					  "12221505");
		AddEntry("WBXasset://fonts/head.mesh",						  "12221462");
		
		AddEntry("WBXasset://fonts/CompositTShirt.mesh",			  "12221404");
		AddEntry("WBXasset://fonts/CompositTorsoBase.mesh",			  "12221396");
		AddEntry("WBXasset://fonts/CompositShirtTemplate.mesh",		  "12221382");
		AddEntry("WBXasset://fonts/CompositRightLegBase.mesh",		  "12221373");
		AddEntry("WBXasset://fonts/CompositRightArmBase.mesh",		  "12221368");
		AddEntry("WBXasset://fonts/CompositPantsTemplate.mesh",		  "12221364");
		AddEntry("WBXasset://fonts/CompositLeftLegBase.mesh",		  "12221356");
		AddEntry("WBXasset://fonts/CompositLeftArmBase.mesh",		  "12221351");

		//AddEntry("WBXasset://fonts/humanoidStatic.WBXm",			  "12221500");
		//AddEntry("WBXasset://fonts/humanoidSound.WBXm",				  "12221490");*/
		AddEntry("WBXasset://fonts/humanoidHealth.WBXm",		      "38037265");
		AddEntry("WBXasset://fonts/humanoidHealthRegenScript.WBXm",		      "11911558");
		/*//AddEntry("WBXasset://fonts/humanoidExtra.WBXm",				  "12221478");
		//AddEntry("WBXasset://fonts/humanoidAnimate.WBXm",			  "12221472");
		//AddEntry("WBXasset://fonts/character.WBXm",					  "12221333");
		//AddEntry("WBXasset://fonts/SlingshotPellet.WBXm",			  "12221691");
		//AddEntry("WBXasset://fonts/Rocket.WBXm",					  "12221636");
		*/															
		
		AddEntry("WBXasset://sounds/bass.wav", "WBXasset://sounds/bass.mp3");
		AddEntry("WBXasset://sounds/button.wav", "WBXasset://sounds/button.mp3");
		AddEntry("WBXasset://sounds/clickfast.wav", "WBXasset://sounds/clickfast.mp3");
		AddEntry("WBXasset://sounds/collide.wav", "WBXasset://sounds/collide.mp3");
		AddEntry("WBXasset://sounds/electronicpingshort.wav", "WBXasset://sounds/electronicpingshort.mp3");
		AddEntry("WBXasset://sounds/flashbulb.wav", "WBXasset://sounds/flashbulb.mp3");
		AddEntry("WBXasset://sounds/grass.ogg", "WBXasset://sounds/grass.mp3");
		AddEntry("WBXasset://sounds/grass2.ogg", "WBXasset://sounds/grass2.mp3");
		AddEntry("WBXasset://sounds/grass3.ogg", "WBXasset://sounds/grass3.mp3");
		AddEntry("WBXasset://sounds/grassstone.ogg", "WBXasset://sounds/grassstone.mp3");
		AddEntry("WBXasset://sounds/grassstone2.ogg", "WBXasset://sounds/grassstone2.mp3");
		AddEntry("WBXasset://sounds/grassstone3.ogg", "WBXasset://sounds/grassstone3.mp3");
		AddEntry("WBXasset://sounds/hit.wav", "WBXasset://sounds/hit.mp3");
		AddEntry("WBXasset://sounds/ice.ogg", "WBXasset://sounds/ice.mp3");
		AddEntry("WBXasset://sounds/ice2.ogg", "WBXasset://sounds/ice2.mp3");
		AddEntry("WBXasset://sounds/ice3.ogg", "WBXasset://sounds/ice3.mp3");
		AddEntry("WBXasset://sounds/icegrass.ogg", "WBXasset://sounds/icegrass.mp3");
		AddEntry("WBXasset://sounds/icegrass2.ogg", "WBXasset://sounds/icegrass2.mp3");
		AddEntry("WBXasset://sounds/icegrass3.ogg", "WBXasset://sounds/icegrass3.mp3");
		AddEntry("WBXasset://sounds/icemetal.ogg", "WBXasset://sounds/icemetal.mp3");
		AddEntry("WBXasset://sounds/icemetal2.ogg", "WBXasset://sounds/icemetal2.mp3");
		AddEntry("WBXasset://sounds/icemetal3.ogg", "WBXasset://sounds/icemetal3.mp3");
		AddEntry("WBXasset://sounds/icestone.ogg", "WBXasset://sounds/icestone.mp3");
		AddEntry("WBXasset://sounds/icestone2.ogg", "WBXasset://sounds/icestone2.mp3");
		AddEntry("WBXasset://sounds/icestone3.ogg", "WBXasset://sounds/icestone3.mp3");
		AddEntry("WBXasset://sounds/Kerplunk.wav", "WBXasset://sounds/Kerplunk.mp3");
		AddEntry("WBXasset://sounds/Kid", "WBXasset://sounds/Kid");
		AddEntry("WBXasset://sounds/metal.ogg", "WBXasset://sounds/metal.mp3");
		AddEntry("WBXasset://sounds/metal2.ogg", "WBXasset://sounds/metal2.mp3");
		AddEntry("WBXasset://sounds/metal3.ogg", "WBXasset://sounds/metal3.mp3");
		AddEntry("WBXasset://sounds/metalgrass.ogg", "WBXasset://sounds/metalgrass.mp3");
		AddEntry("WBXasset://sounds/metalgrass2.ogg", "WBXasset://sounds/metalgrass2.mp3");
		AddEntry("WBXasset://sounds/metalgrass3.ogg", "WBXasset://sounds/metalgrass3.mp3");
		AddEntry("WBXasset://sounds/metalstone.ogg", "WBXasset://sounds/metalstone.mp3");
		AddEntry("WBXasset://sounds/metalstone2.ogg", "WBXasset://sounds/metalstone2.mp3");
		AddEntry("WBXasset://sounds/metalstone3.ogg", "WBXasset://sounds/metalstone3.mp3");
		AddEntry("WBXasset://sounds/pageturn.wav", "WBXasset://sounds/pageturn.mp3");
		AddEntry("WBXasset://sounds/plasticgrass.ogg", "WBXasset://sounds/plasticgrass.mp3");
		AddEntry("WBXasset://sounds/plasticgrass2.ogg", "WBXasset://sounds/plasticgrass2.mp3");
		AddEntry("WBXasset://sounds/plasticgrass3.ogg", "WBXasset://sounds/plasticgrass3.mp3");
		AddEntry("WBXasset://sounds/plasticice.ogg", "WBXasset://sounds/plasticice.mp3");
		AddEntry("WBXasset://sounds/plasticice2.ogg", "WBXasset://sounds/plasticice2.mp3");
		AddEntry("WBXasset://sounds/plasticice3.ogg", "WBXasset://sounds/plasticice3.mp3");
		AddEntry("WBXasset://sounds/plasticmetal.ogg", "WBXasset://sounds/plasticmetal.mp3");
		AddEntry("WBXasset://sounds/plasticmetal2.ogg", "WBXasset://sounds/plasticmetal2.mp3");
		AddEntry("WBXasset://sounds/plasticmetal3.ogg", "WBXasset://sounds/plasticmetal3.mp3");
		AddEntry("WBXasset://sounds/plasticplastic.ogg", "WBXasset://sounds/plasticplastic.mp3");
		AddEntry("WBXasset://sounds/plasticplastic2.ogg", "WBXasset://sounds/plasticplastic2.mp3");
		AddEntry("WBXasset://sounds/plasticplastic3.ogg", "WBXasset://sounds/plasticplastic3.mp3");
		AddEntry("WBXasset://sounds/plasticstone.ogg", "WBXasset://sounds/plasticstone.mp3");
		AddEntry("WBXasset://sounds/plasticstone2.ogg", "WBXasset://sounds/plasticstone2.mp3");
		AddEntry("WBXasset://sounds/plasticstone3.ogg", "WBXasset://sounds/plasticstone3.mp3");
		AddEntry("WBXasset://sounds/Rubber", "WBXasset://sounds/Rubber");
		AddEntry("WBXasset://sounds/Rubber", "WBXasset://sounds/Rubber");
		AddEntry("WBXasset://sounds/snap.wav", "WBXasset://sounds/snap.mp3");
		AddEntry("WBXasset://sounds/splat.wav", "WBXasset://sounds/splat.mp3");
		AddEntry("WBXasset://sounds/stone.ogg", "WBXasset://sounds/stone.mp3");
		AddEntry("WBXasset://sounds/stone2.ogg", "WBXasset://sounds/stone2.mp3");
		AddEntry("WBXasset://sounds/stone3.ogg", "WBXasset://sounds/stone3.mp3");
		AddEntry("WBXasset://sounds/switch.wav", "WBXasset://sounds/switch.mp3");
		AddEntry("WBXasset://sounds/SWITCH3.wav", "WBXasset://sounds/SWITCH3.mp3");
		AddEntry("WBXasset://sounds/swoosh.wav", "WBXasset://sounds/swoosh.mp3");
		AddEntry("WBXasset://sounds/swordlunge.wav", "WBXasset://sounds/swordlunge.mp3");
		AddEntry("WBXasset://sounds/swordslash.wav", "WBXasset://sounds/swordslash.mp3");
		AddEntry("WBXasset://sounds/unsheath.wav", "WBXasset://sounds/unsheath.mp3");
		AddEntry("WBXasset://sounds/uuhhh.wav", "WBXasset://sounds/uuhhh.mp3");
		AddEntry("WBXasset://sounds/victory.wav", "WBXasset://sounds/victory.mp3");
		AddEntry("WBXasset://sounds/woodgrass.ogg", "WBXasset://sounds/woodgrass.mp3");
		AddEntry("WBXasset://sounds/woodgrass2.ogg", "WBXasset://sounds/woodgrass2.mp3");
		AddEntry("WBXasset://sounds/woodgrass3.ogg", "WBXasset://sounds/woodgrass3.mp3");
		AddEntry("WBXasset://sounds/woodice.ogg", "WBXasset://sounds/woodice.mp3");
		AddEntry("WBXasset://sounds/woodice2.ogg", "WBXasset://sounds/woodice2.mp3");
		AddEntry("WBXasset://sounds/woodice3.ogg", "WBXasset://sounds/woodice3.mp3");
		AddEntry("WBXasset://sounds/woodmetal.ogg", "WBXasset://sounds/woodmetal.mp3");
		AddEntry("WBXasset://sounds/woodmetal2.ogg", "WBXasset://sounds/woodmetal2.mp3");
		AddEntry("WBXasset://sounds/woodmetal3.ogg", "WBXasset://sounds/woodmetal3.mp3");
		AddEntry("WBXasset://sounds/woodplastic.ogg", "WBXasset://sounds/woodplastic.mp3");
		AddEntry("WBXasset://sounds/woodplastic2.ogg", "WBXasset://sounds/woodplastic2.mp3");
		AddEntry("WBXasset://sounds/woodplastic3.ogg", "WBXasset://sounds/woodplastic3.mp3");
		AddEntry("WBXasset://sounds/woodstone.ogg", "WBXasset://sounds/woodstone.mp3");
		AddEntry("WBXasset://sounds/woodstone2.ogg", "WBXasset://sounds/woodstone2.mp3");
		AddEntry("WBXasset://sounds/woodstone3.ogg", "WBXasset://sounds/woodstone3.mp3");
		AddEntry("WBXasset://sounds/woodwood.ogg", "WBXasset://sounds/woodwood.mp3");
		AddEntry("WBXasset://sounds/woodwood2.ogg", "WBXasset://sounds/woodwood2.mp3");
		AddEntry("WBXasset://sounds/woodwood3.ogg", "WBXasset://sounds/woodwood3.mp3");

		if (FFlag::DebugRenderDownloadAssets)
        {
        #if defined(__ANDROID__)
            // Note: the assets that are actually used on Android are loaded from local files
            // We don't have on-disk cache so loading them off CDN would mean poor user experience.
            AddEntryProd("WBXasset://textures/aluminum/diffuse.dds", "153465459");
            AddEntryProd("WBXasset://textures/aluminum/normal.dds", "153465465");
            AddEntryProd("WBXasset://textures/aluminum/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/aluminum/specular.dds", "153465469");
            AddEntryProd("WBXasset://textures/brick/diffuse.dds", "161854081");
            AddEntryProd("WBXasset://textures/brick/normal.dds", "161854090");
            AddEntryProd("WBXasset://textures/brick/normaldetail.dds", "161854091");
            AddEntryProd("WBXasset://textures/brick/specular.dds", "161854095");
            AddEntryProd("WBXasset://textures/concrete/diffuse.dds", "153465490");
            AddEntryProd("WBXasset://textures/concrete/normal.dds", "153465494");
            AddEntryProd("WBXasset://textures/concrete/normaldetail.dds", "153465498");
            AddEntryProd("WBXasset://textures/concrete/specular.dds", "153465503");
            AddEntryProd("WBXasset://textures/diamondplate/diffuse.dds", "153465505");
            AddEntryProd("WBXasset://textures/diamondplate/normal.dds", "153465513");
            AddEntryProd("WBXasset://textures/diamondplate/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/diamondplate/specular.dds", "153465517");
            AddEntryProd("WBXasset://textures/fabric/diffuse.dds", "153465528");
            AddEntryProd("WBXasset://textures/fabric/normal.dds", "153465544");
            AddEntryProd("WBXasset://textures/fabric/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/fabric/specular.dds", "153465548");
            AddEntryProd("WBXasset://textures/granite/diffuse.dds", "153465554");
            AddEntryProd("WBXasset://textures/granite/normal.dds", "153465558");
            AddEntryProd("WBXasset://textures/granite/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/granite/specular.dds", "153465565");
            AddEntryProd("WBXasset://textures/grass/diffuse.dds", "153465576");
            AddEntryProd("WBXasset://textures/grass/normal.dds", "153465584");
            AddEntryProd("WBXasset://textures/grass/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/grass/specular.dds", "153465592");
            AddEntryProd("WBXasset://textures/ice/diffuse.dds", "153465459");
            AddEntryProd("WBXasset://textures/ice/normal.dds", "153465600");
            AddEntryProd("WBXasset://textures/ice/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/ice/specular.dds", "153465605");
            AddEntryProd("WBXasset://textures/marble/diffuse.dds", "153465612");
            AddEntryProd("WBXasset://textures/marble/normal.dds", "153465616");
            AddEntryProd("WBXasset://textures/marble/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/marble/specular.dds", "153465622");
            AddEntryProd("WBXasset://textures/pebble/diffuse.dds", "153465626");
            AddEntryProd("WBXasset://textures/pebble/normal.dds", "153465467");
            AddEntryProd("WBXasset://textures/pebble/normaldetail.dds", "153465498");
            AddEntryProd("WBXasset://textures/pebble/specular.dds", "153465503");
            AddEntryProd("WBXasset://textures/plastic/diffuse.dds", "153465628");
            AddEntryProd("WBXasset://textures/plastic/normal.dds", "153465633");
            AddEntryProd("WBXasset://textures/plastic/normaldetail.dds", "153465640");
            AddEntryProd("WBXasset://textures/rust/diffuse.dds", "153465645");
            AddEntryProd("WBXasset://textures/rust/normal.dds", "153465657");
            AddEntryProd("WBXasset://textures/rust/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/rust/specular.dds", "153465665");
            AddEntryProd("WBXasset://textures/sand/diffuse.dds", "153465671");
            AddEntryProd("WBXasset://textures/sand/normal.dds", "153465677");
            AddEntryProd("WBXasset://textures/sand/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/sand/specular.dds", "153465684");
            // AddEntryProd("WBXasset://textures/sky/sky512_bk.tex", "153465690");
            // AddEntryProd("WBXasset://textures/sky/sky512_dn.tex", "153465700");
            // AddEntryProd("WBXasset://textures/sky/sky512_ft.tex", "153465708");
            // AddEntryProd("WBXasset://textures/sky/sky512_lf.tex", "153465714");
            // AddEntryProd("WBXasset://textures/sky/sky512_rt.tex", "153465716");
            // AddEntryProd("WBXasset://textures/sky/sky512_up.tex", "153465724");
            AddEntryProd("WBXasset://textures/slate/diffuse.dds", "153465732");
            AddEntryProd("WBXasset://textures/slate/normal.dds", "153465749");
            AddEntryProd("WBXasset://textures/slate/normaldetail.dds", "153465755");
            AddEntryProd("WBXasset://textures/slate/specular.dds", "153465758");
            // AddEntryProd("WBXasset://textures/terrain/diffuse.dds", "153482859");
            // AddEntryProd("WBXasset://textures/terrain/diffusefar.dds", "153482879");
            // AddEntryProd("WBXasset://textures/terrain/normal.dds", "153482917");
            // AddEntryProd("WBXasset://textures/terrain/specular.dds", "153482944");
            AddEntryProd("WBXasset://textures/wood/diffuse.dds", "153465837");
            AddEntryProd("WBXasset://textures/wood/normal.dds", "153465842");
            AddEntryProd("WBXasset://textures/wood/normaldetail.dds", "153465844");
            AddEntryProd("WBXasset://textures/wood/specular.dds", "153465850");

            AddEntryProd("WBXasset://textures/cobblestone/diffuse.dds", "161854129");
            AddEntryProd("WBXasset://textures/cobblestone/normal.dds", "161854138");
            AddEntryProd("WBXasset://textures/cobblestone/normaldetail.dds", "161854143");
            AddEntryProd("WBXasset://textures/cobblestone/specular.dds", "161854147");
            AddEntryProd("WBXasset://textures/metal/diffuse.dds", "161854181");
            AddEntryProd("WBXasset://textures/metal/normal.dds", "161854188");
            AddEntryProd("WBXasset://textures/metal/normaldetail.dds", "161854194");
            AddEntryProd("WBXasset://textures/metal/specular.dds", "161854196");
            AddEntryProd("WBXasset://textures/woodplanks/diffuse.dds", "161854218");
            AddEntryProd("WBXasset://textures/woodplanks/normal.dds", "161854225");
            AddEntryProd("WBXasset://textures/woodplanks/normaldetail.dds", "161854226");
            AddEntryProd("WBXasset://textures/woodplanks/specular.dds", "161854228");
        #elif defined(WBX_PLATFORM_IOS)
            // Note: the assets that are actually used on iOS are loaded from local files
            // We don't have on-disk cache so loading them off CDN would mean poor user experience.
            AddEntryProd("WBXasset://textures/aluminum/diffuse.pvr", "153470177");
            AddEntryProd("WBXasset://textures/aluminum/normal.pvr", "153470185");
            AddEntryProd("WBXasset://textures/aluminum/normaldetail.pvr", "153470186");
            AddEntryProd("WBXasset://textures/aluminum/specular.pvr", "153470187");
            AddEntryProd("WBXasset://textures/brick/diffuse.pvr", "161854295");
            AddEntryProd("WBXasset://textures/brick/normal.pvr", "161854298");
            AddEntryProd("WBXasset://textures/brick/normaldetail.pvr", "161854302");
            AddEntryProd("WBXasset://textures/brick/specular.pvr", "161854304");
            AddEntryProd("WBXasset://textures/concrete/diffuse.pvr", "153470207");
            AddEntryProd("WBXasset://textures/concrete/normal.pvr", "153470212");
            AddEntryProd("WBXasset://textures/concrete/normaldetail.pvr", "153470218");
            AddEntryProd("WBXasset://textures/concrete/specular.pvr", "153470220");
            AddEntryProd("WBXasset://textures/diamondplate/diffuse.pvr", "153470224");
            AddEntryProd("WBXasset://textures/diamondplate/normal.pvr", "153470225");
            AddEntryProd("WBXasset://textures/diamondplate/normaldetail.pvr", "153470186");
            AddEntryProd("WBXasset://textures/diamondplate/specular.pvr", "153470229");
            AddEntryProd("WBXasset://textures/fabric/diffuse.pvr", "153470230");
            AddEntryProd("WBXasset://textures/fabric/normal.pvr", "153470232");
            AddEntryProd("WBXasset://textures/fabric/normaldetail.pvr", "153470186");
            AddEntryProd("WBXasset://textures/fabric/specular.pvr", "153470242");
            AddEntryProd("WBXasset://textures/granite/diffuse.pvr", "153470246");
            AddEntryProd("WBXasset://textures/granite/normal.pvr", "153470247");
            AddEntryProd("WBXasset://textures/granite/normaldetail.pvr", "153470186");
            AddEntryProd("WBXasset://textures/granite/specular.pvr", "153470252");
            AddEntryProd("WBXasset://textures/grass/diffuse.pvr", "153470263");
            AddEntryProd("WBXasset://textures/grass/normal.pvr", "153470269");
            AddEntryProd("WBXasset://textures/grass/normaldetail.pvr", "153470186");
            AddEntryProd("WBXasset://textures/grass/specular.pvr", "153470273");
            AddEntryProd("WBXasset://textures/ice/diffuse.pvr", "153470177");
            AddEntryProd("WBXasset://textures/ice/normal.pvr", "153470279");
            AddEntryProd("WBXasset://textures/ice/normaldetail.pvr", "153470186");
            AddEntryProd("WBXasset://textures/ice/specular.pvr", "153470283");
            AddEntryProd("WBXasset://textures/marble/diffuse.pvr", "153470287");
            AddEntryProd("WBXasset://textures/marble/normal.pvr", "153470247");
            AddEntryProd("WBXasset://textures/marble/normaldetail.pvr", "153470186");
            AddEntryProd("WBXasset://textures/marble/specular.pvr", "153470290");
            AddEntryProd("WBXasset://textures/pebble/diffuse.pvr", "153470291");
            AddEntryProd("WBXasset://textures/pebble/normal.pvr", "153470186");
            AddEntryProd("WBXasset://textures/pebble/normaldetail.pvr", "153470218");
            AddEntryProd("WBXasset://textures/pebble/specular.pvr", "153470220");
            AddEntryProd("WBXasset://textures/plastic/diffuse.dds", "153470298");
            AddEntryProd("WBXasset://textures/plastic/normal.dds", "153470304");
            AddEntryProd("WBXasset://textures/plastic/normaldetail.pvr", "153470307");
            AddEntryProd("WBXasset://textures/rust/diffuse.pvr", "153470310");
            AddEntryProd("WBXasset://textures/rust/normal.pvr", "153470315");
            AddEntryProd("WBXasset://textures/rust/normaldetail.pvr", "153470186");
            AddEntryProd("WBXasset://textures/rust/specular.pvr", "153470319");
            AddEntryProd("WBXasset://textures/sand/diffuse.pvr", "153470322");
            AddEntryProd("WBXasset://textures/sand/normal.pvr", "153470324");
            AddEntryProd("WBXasset://textures/sand/normaldetail.pvr", "153470186");
            AddEntryProd("WBXasset://textures/sand/specular.pvr", "153470328");
            // AddEntryProd("WBXasset://textures/sky/sky512_bk.tex", "153470335");
            // AddEntryProd("WBXasset://textures/sky/sky512_dn.tex", "153470337");
            // AddEntryProd("WBXasset://textures/sky/sky512_ft.tex", "153470340");
            // AddEntryProd("WBXasset://textures/sky/sky512_lf.tex", "153470342");
            // AddEntryProd("WBXasset://textures/sky/sky512_rt.tex", "153470344");
            // AddEntryProd("WBXasset://textures/sky/sky512_up.tex", "153470348");
            AddEntryProd("WBXasset://textures/slate/diffuse.pvr", "153470351");
            AddEntryProd("WBXasset://textures/slate/normal.pvr", "153470359");
            AddEntryProd("WBXasset://textures/slate/normaldetail.pvr", "153470362");
            AddEntryProd("WBXasset://textures/slate/specular.pvr", "153470366");
            // AddEntryProd("WBXasset://textures/terrain/diffuse.pvr", "153483079");
            // AddEntryProd("WBXasset://textures/terrain/diffusefar.pvr", "153483088");
            // AddEntryProd("WBXasset://textures/terrain/normal.pvr", "153483105");
            // AddEntryProd("WBXasset://textures/terrain/specular.pvr", "153483123");
            AddEntryProd("WBXasset://textures/wood/diffuse.pvr", "153470405");
            AddEntryProd("WBXasset://textures/wood/normal.pvr", "153470410");
            AddEntryProd("WBXasset://textures/wood/normaldetail.pvr", "153470412");
            AddEntryProd("WBXasset://textures/wood/specular.pvr", "153470415");

            AddEntryProd("WBXasset://textures/cobblestone/diffuse.pvr", "161854333");
            AddEntryProd("WBXasset://textures/cobblestone/normal.pvr", "161854340");
            AddEntryProd("WBXasset://textures/cobblestone/normaldetail.pvr", "161854343");
            AddEntryProd("WBXasset://textures/cobblestone/specular.pvr", "161854345");
            AddEntryProd("WBXasset://textures/metal/diffuse.pvr", "161854366");
            AddEntryProd("WBXasset://textures/metal/normal.pvr", "161854370");
            AddEntryProd("WBXasset://textures/metal/normaldetail.pvr", "161854371");
            AddEntryProd("WBXasset://textures/metal/specular.pvr", "161854373");
            AddEntryProd("WBXasset://textures/woodplanks/diffuse.pvr", "161854398");
            AddEntryProd("WBXasset://textures/woodplanks/normal.pvr", "161854403");
            AddEntryProd("WBXasset://textures/woodplanks/normaldetail.pvr", "161854404");
            AddEntryProd("WBXasset://textures/woodplanks/specular.pvr", "161854405");
        #else
            AddEntryProd("WBXasset://textures/aluminum/diffuse.dds", "153465459");
            AddEntryProd("WBXasset://textures/aluminum/normal.dds", "153465465");
            AddEntryProd("WBXasset://textures/aluminum/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/aluminum/specular.dds", "153465469");
            AddEntryProd("WBXasset://textures/brick/diffuse.dds", "161854081");
            AddEntryProd("WBXasset://textures/brick/normal.dds", "161854090");
            AddEntryProd("WBXasset://textures/brick/normaldetail.dds", "161854091");
            AddEntryProd("WBXasset://textures/brick/specular.dds", "161854095");
            AddEntryProd("WBXasset://textures/concrete/diffuse.dds", "153465490");
            AddEntryProd("WBXasset://textures/concrete/normal.dds", "153465494");
            AddEntryProd("WBXasset://textures/concrete/normaldetail.dds", "153465498");
            AddEntryProd("WBXasset://textures/concrete/specular.dds", "153465503");
            AddEntryProd("WBXasset://textures/diamondplate/diffuse.dds", "153465505");
            AddEntryProd("WBXasset://textures/diamondplate/normal.dds", "153465513");
            AddEntryProd("WBXasset://textures/diamondplate/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/diamondplate/specular.dds", "153465517");
            AddEntryProd("WBXasset://textures/fabric/diffuse.dds", "153465528");
            AddEntryProd("WBXasset://textures/fabric/normal.dds", "153465544");
            AddEntryProd("WBXasset://textures/fabric/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/fabric/specular.dds", "153465548");
            AddEntryProd("WBXasset://textures/granite/diffuse.dds", "153465554");
            AddEntryProd("WBXasset://textures/granite/normal.dds", "153465558");
            AddEntryProd("WBXasset://textures/granite/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/granite/specular.dds", "153465565");
            AddEntryProd("WBXasset://textures/grass/diffuse.dds", "153465576");
            AddEntryProd("WBXasset://textures/grass/normal.dds", "153465584");
            AddEntryProd("WBXasset://textures/grass/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/grass/specular.dds", "153465592");
            AddEntryProd("WBXasset://textures/ice/diffuse.dds", "153465459");
            AddEntryProd("WBXasset://textures/ice/normal.dds", "153465600");
            AddEntryProd("WBXasset://textures/ice/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/ice/specular.dds", "153465605");
            AddEntryProd("WBXasset://textures/marble/diffuse.dds", "153465612");
            AddEntryProd("WBXasset://textures/marble/normal.dds", "153465616");
            AddEntryProd("WBXasset://textures/marble/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/marble/specular.dds", "153465622");
            AddEntryProd("WBXasset://textures/pebble/diffuse.dds", "153465626");
            AddEntryProd("WBXasset://textures/pebble/normal.dds", "153465467");
            AddEntryProd("WBXasset://textures/pebble/normaldetail.dds", "153465498");
            AddEntryProd("WBXasset://textures/pebble/specular.dds", "153465503");
            AddEntryProd("WBXasset://textures/plastic/diffuse.dds", "153465628");
            AddEntryProd("WBXasset://textures/plastic/normal.dds", "153465633");
            AddEntryProd("WBXasset://textures/plastic/normaldetail.dds", "153465640");
            AddEntryProd("WBXasset://textures/rust/diffuse.dds", "153465645");
            AddEntryProd("WBXasset://textures/rust/normal.dds", "153465657");
            AddEntryProd("WBXasset://textures/rust/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/rust/specular.dds", "153465665");
            AddEntryProd("WBXasset://textures/sand/diffuse.dds", "153465671");
            AddEntryProd("WBXasset://textures/sand/normal.dds", "153465677");
            AddEntryProd("WBXasset://textures/sand/normaldetail.dds", "153465467");
            AddEntryProd("WBXasset://textures/sand/specular.dds", "153465684");
            AddEntryProd("WBXasset://textures/sky/sky512_bk.tex", "153465690");
            AddEntryProd("WBXasset://textures/sky/sky512_dn.tex", "153465700");
            AddEntryProd("WBXasset://textures/sky/sky512_ft.tex", "153465708");
            AddEntryProd("WBXasset://textures/sky/sky512_lf.tex", "153465714");
            AddEntryProd("WBXasset://textures/sky/sky512_rt.tex", "153465716");
            AddEntryProd("WBXasset://textures/sky/sky512_up.tex", "153465724");
            AddEntryProd("WBXasset://textures/slate/diffuse.dds", "153465732");
            AddEntryProd("WBXasset://textures/slate/normal.dds", "153465749");
            AddEntryProd("WBXasset://textures/slate/normaldetail.dds", "153465755");
            AddEntryProd("WBXasset://textures/slate/specular.dds", "153465758");
            AddEntryProd("WBXasset://textures/terrain/diffuse.dds", "153482859");
            AddEntryProd("WBXasset://textures/terrain/diffusefar.dds", "153482879");
            AddEntryProd("WBXasset://textures/terrain/normal.dds", "153482917");
            AddEntryProd("WBXasset://textures/terrain/specular.dds", "153482944");
            AddEntryProd("WBXasset://textures/wood/diffuse.dds", "153465837");
            AddEntryProd("WBXasset://textures/wood/normal.dds", "153465842");
            AddEntryProd("WBXasset://textures/wood/normaldetail.dds", "153465844");
            AddEntryProd("WBXasset://textures/wood/specular.dds", "153465850");

            AddEntryProd("WBXasset://textures/cobblestone/diffuse.dds", "161854129");
            AddEntryProd("WBXasset://textures/cobblestone/normal.dds", "161854138");
            AddEntryProd("WBXasset://textures/cobblestone/normaldetail.dds", "161854143");
            AddEntryProd("WBXasset://textures/cobblestone/specular.dds", "161854147");
            AddEntryProd("WBXasset://textures/metal/diffuse.dds", "161854181");
            AddEntryProd("WBXasset://textures/metal/normal.dds", "161854188");
            AddEntryProd("WBXasset://textures/metal/normaldetail.dds", "161854194");
            AddEntryProd("WBXasset://textures/metal/specular.dds", "161854196");
            AddEntryProd("WBXasset://textures/woodplanks/diffuse.dds", "161854218");
            AddEntryProd("WBXasset://textures/woodplanks/normal.dds", "161854225");
            AddEntryProd("WBXasset://textures/woodplanks/normaldetail.dds", "161854226");
            AddEntryProd("WBXasset://textures/woodplanks/specular.dds", "161854228");
        #endif
        }
	}
	
	void LegacyContentTable::AddEntry(const std::string& path, const std::string& contentId)
	{
		std::string url = path;
		normalizeUrl(url);
		
		mMap[url] = contentId;
	}
	
	void LegacyContentTable::AddEntryProd(const std::string& path, const std::string& contentId)
	{
		std::string url = path;
		normalizeUrl(url);
		
		mMap[url] = format("http://www.freblx.xyz/Asset?id=%s", contentId.c_str());
	}
	
	const std::string& LegacyContentTable::FindEntry(const std::string& path)
	{
		std::string url = path;
		normalizeUrl(url);
		
		UrlMap::const_iterator it = mMap.find(url);
		
		return (it == mMap.end()) ? mEmpty : it->second;
	}
}

