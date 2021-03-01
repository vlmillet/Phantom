
Phantom_lib("Phantom",  { } -- DEPENDENCIES
	,
	function(Vars) -- include public
		filter {"toolset:msc"}
			buildoptions { "/vmg /vmv" }
		filter {}
		
		if(Vars["Link"] == "Static") then
			defines { "PHANTOM_STATIC_LINK_PHANTOM" }
		end

	end
	, 
	function(Vars)  -- include private
		defines
		{
			"PHANTOM_LIB_PHANTOM",
			"_CRT_SECURE_NO_WARNINGS"
		}
		
		files { "./natvis/Phantom.natvis" }
					
        Phantom_pch()
			
		filter {}

		local hauntParams = {}
		hauntParams["IsHaunted"] = true
		Phantom_plugin("Phantom", Vars, hauntParams)

	end
	,
	function(Vars) -- link
		filter { "platforms:Win32 or x64" }
			links {
				"DbgHelp.lib"
			}

		filter {}
	end

)
