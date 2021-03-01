-- Here is an example of the most common used configuration functions
-- For more in-depth use of premake, refer to the online documentation:
-- https://github.com/premake/premake-core/wiki

include "extend"

newoption {
   trigger     = "haunt-options",
   description = "Specify haunt extra options to apply to all projects using phantom"
}

Phantom_projects_data_ = {}

Phantom_current_project_ = ""
Phantom_current_vars_ = {}

Phantom_current_filter_ = {}

_premake_includedirs = includedirs
_premake_filter = filter

function insert_unique(t, v)
	t[v] = true
end

-- OVERWRITE PREMAKE FUNCTIONS ================================================================================================================================

function includedirs(param)
	_premake_includedirs(param)
	if(Phantom_current_project_ == "") then
		return
	end

	local win = false
	if(#Phantom_current_filter_ == 0) then
		win = true
	else 
		for k,v in ipairs(Phantom_current_filter_) do
			if(v:find("platforms") == nil) then
				win = true
				break;
			elseif(v:find("Win32") ~= nil or v:find("x64") ~= nil) then
				win = true 
				break
			end
		end
	end
	if(win) then
		-- print("WIN TRUE =====================================")
		if(type(param) == "table") then
			for k,v in ipairs(param) do
				-- print(Phantom_current_project_ .. " " .. v)
				insert_unique(Phantom_projects_data_[Phantom_current_project_]["includedirs"], v)
			end
		else 
			insert_unique(Phantom_projects_data_[Phantom_current_project_]["includedirs"], param)
		end
	else
		-- print("WIN FALSE =====================================")
	end 
end

function filter(param)
	_premake_filter(param)
	if(Phantom_current_project_ == "") then
		return
	end
	if(type(param) == "table") then
		Phantom_current_filter_ = param
		-- for k,v in ipairs(param) do
			-- print(v)
		-- end
	else
		Phantom_current_filter_ = {}
		table.insert(Phantom_current_filter_, param)
		-- print(param)
	end
end

-- PHANTOM HELPER FUNCTIONS ================================================================================================================================ 

function HauntIncludeCmdArgs(Vars)
	local cmdArgs = ""
	for v,_ in pairs(Phantom_includedirs()) do
		if(string.find(v, "`") == nil) then
			cmdArgs = cmdArgs .. " -I" .. path.getrelative(Phantom_solution_path(Vars), v) 
		end
	end
	return cmdArgs
end

function Phantom_current_lua_folder()
   local str = debug.getinfo(2, "S").source:sub(2)
   return str:match("(.*[/\\])") or "."
end

function Phantom_project_data(key)
	return Phantom_projects_data_[Phantom_current_project_][key]
end

function AutoHaunt(Vars, HauntOptions, HauntPath, WarningPushPop)

	if(HauntPath == nil) then
		HauntPath = path.getrelative(Phantom_solution_path(Vars), Phantom_project_data("SourcePath")) 
	else
		HauntPath = path.getrelative(Phantom_solution_path(Vars), HauntPath) 
	end 
	
	local cmdArgs = HauntPath .. HauntIncludeCmdArgs(Vars)
	
	if(HauntOptions ~= nil) then 
		cmdArgs = cmdArgs .. " " .. HauntOptions
	end

	local FullInputPath = path.getabsolute(Phantom_project_data("SourcePath"))
	
	Haunt(FullInputPath, cmdArgs, WarningPushPop)
	
end

function Haunt(FullInputPath, HauntOptions, WarningPushPop)
	
	if(os.getenv("DONT_HAUNT") == "true") then
		return     -- We don't want to call Haunt on Jenkins because we want to test the versioned hxx
	end
	
    local exe = Phantom_current_lua_folder() .. "../tools/haunt/haunt.exe"
	local exe_win = path.translate(exe)
	-- prebuildcommands {
		--  exe_win .. " " .. HauntOptions
	-- }


	-- we use .lua project file as a hook to launch haunt (there is no lua implied in the haunt process)
	filter { "files:**.lua" }
	   -- A message to display while this build step is running (optional)
		buildmessage '================== Haunting project %{file.name} =================='

	   -- One or more commands to run (required)
	    hauntCommand = exe_win .. " " .. HauntOptions,
		buildcommands {
		  -- hauntCommand,
		}

		file = io.open("Haunt.bat", "a")
		file:write(hauntCommand .. "\n")
		
		local pushWarningExe = Phantom_current_lua_folder() .. "../tools/PushWarning/PushWarning.exe"
		pushWarningCommand = path.translate(pushWarningExe) .. " " .. FullInputPath
		if WarningPushPop then
			-- buildcommands { pushWarningCommand }
			file:write(pushWarningCommand .. "\n")
		end

		file:close()
		
	   -- One or more outputs resulting from the build (required)
		buildoutputs { 'no_output' }

	   -- One or more additional dependencies for this build command (optional)
		buildinputs { 'no_input' }

		-- print(exe_win .. " " .. FullInputPath .. " --cache-clear")
		file = io.open("HauntClearCache.bat", "a")
		file:write(exe_win .. " " .. FullInputPath .. " --cache-clear\n")
		file:close()

	filter {}
	
end

-- HauntParams:
--		IsHaunted:bool
--		HxxFilterFunc:bool func(str)
function Phantom_plugin(Name, Vars, HauntParams, HauntOptions, HauntPath)
	
	local upper = string.upper(Name)
	upper = string.gsub(upper, "%.", "_")

	if(Vars["Link"] == "Static") then
		defines { "PHANTOM_STATIC_LINK_PHANTOM", "PHANTOM_STATIC_LINK_" .. upper, "PHANTOM_STATIC_PLUGIN=" .. string.hash(Name) }
	end
	if(HauntParams.IsHaunted==true) then

		local cmdArgs = " -c " .. Vars["HauntOptions"]
		if(HauntOptions ~= nil) then
			cmdArgs = cmdArgs .. " " .. HauntOptions
		end
		if HauntParams.WarningPushPop == nil then
			HauntParams.WarningPushPop = true
		end
		AutoHaunt(Vars, cmdArgs, HauntPath, HauntParams.WarningPushPop)
		-- print(cmdArgs)
	end
end

function Phantom_app(Name, Vars, HauntParams)
	Phantom_plugin(Name, Vars, HauntParams)
end 


function Phantom_file_exists(filename)
	local fd=io.open(filename,"r")
	if fd~=nil then
		io.close(fd)
		return true
	else
		return false
	end
end

function Phantom_get_current_dir()
	local fdesc = io.popen("cd")

	if fdesc then
		local curdir = fdesc:read("*l")
		return string.gsub(curdir, "(\\)", "/")
	end

	return nil
end

function Phantom_tab_contains(table, value)
    for _i, _v in ipairs(table) do
        if _v == value then
            return true
        end
    end

    return false
end

function string.starts(String,Start)
   return string.sub(String,1,string.len(Start))==Start
end

function string.ends(String,End)
   return End=='' or string.sub(String,-string.len(End))==End
end

function Phantom_includedirs()
	return Phantom_projects_data_[Phantom_current_project_]["includedirs"]
end

local Phantom_dbg_indentation = 0

function _Phantom_dbg_indent(count)
	Phantom_dbg_indentation = Phantom_dbg_indentation + count
end

function _Phantom_dbg_print(text)
	
	local tabs = Phantom_dbg_indentation;

	for i = 1, tabs, 1
	do
	   io.write("  ")
	end
	io.write(text)
	io.write("\n")

end

function _Phantom_add_dependency_static(PhantomRootPath, Name)
	defines { "PHANTOM_STATIC_LINK_" .. string.gsub(string.upper(Name), "\\.", "_") }
end

function Phantom_static_lib_add_dependency(PhantomRootPath, Name, is_static_dependency)
	Phantom_use_includedirs(PhantomRootPath, Name)
	if(is_static_dependency) then 
		_Phantom_add_dependency_static(PhantomRootPath, Name)
	end 
end	

function Phantom_executable_or_shared_lib_add_dependency(PhantomRootPath, Name, is_static_dependency)
	-- print("phantom_add_executable_dependency : " .. PhantomRootPath .. " , " .. Name)
	Phantom_use_includedirs(PhantomRootPath, Name)
	Phantom_use_libdirs(PhantomRootPath, Name)
	if(is_static_dependency) then 
		_Phantom_add_dependency_static(PhantomRootPath, Name)
	end 
	dependson { Name }
end	

function Phantom_define_project(Name, dependencies, func_public, func_private, func_link, exe)
	Phantom_projects_data_[Name] = {}
	Phantom_projects_data_[Name]["public"] = func_public
	Phantom_projects_data_[Name]["private"] = func_private
	Phantom_projects_data_[Name]["link"] = func_link
	Phantom_projects_data_[Name]["dependencies"] = dependencies
	Phantom_projects_data_[Name]["exe"] = exe
	Phantom_projects_data_[Name]["includedirs"] = {}
	Phantom_projects_data_[Name]["SourcePath"] = ""
end

function Phantom_console_app(Name, dependencies, func_private)
	Phantom_define_project(Name, dependencies, function(Vars) end, func_private, function(Vars) end, true)
end

function Phantom_lib(Name, dependencies, func_public, func_private, func_link)
	Phantom_define_project(Name, dependencies, func_public, func_private, func_link, false)
end

function _Phantom_get_current_includedirs()
	local __my_prj = project()
	return _Phantom_get_includedirs(__my_prj.Name)
end

function _Phantom_project_folder(Name, Vars)

	for i,folder in ipairs(Vars["ProjectSearchPaths"]) do
		if(Phantom_file_exists(folder .. "/" .. Name .. "/" .. Name .. ".premake.lua")) then
            return folder
        end
	end
	return nil

end

function _Phantom_include(Name, Vars)

	local folder = _Phantom_project_folder(Name, Vars)
	if(folder ~= nil) then
		include(folder .. "/" .. Name .. "/" .. Name .. ".premake")
	else 
		error("no project '".. Name .."' found in the given search folders (project file must be like '<search-folder>/MyProject/MyProject.premake.lua'")
		return
	end
	return folder

end

function Phantom_solution_path(Vars)
	return "./build/" .. Vars["SolutionName"] .. "/" .. _ACTION .. "/"
end

-- .. "%{cfg.platform}/%{cfg.buildcfg}/"

function Phantom_project_output_path(Name, Vars)

	local WorkspacePath = Vars["WorkspacePath"]

	-- Normalize path
	if not string.endswith(WorkspacePath, "/") then
		WorkspacePath = WorkspacePath .. "/"
	end

	local OutputPathBase = Phantom_solution_path(Vars)
	
	if(Phantom_projects_data_[Name]["exe"]) then
		return OutputPathBase .. "$(Platform)/$(Configuration)/bin/"
	else 
		return OutputPathBase .. "$(Platform)/$(Configuration)/lib/"
	end
	
end

function _Phantom_add_dependency(Name, Dependency, Vars)

	filter {}
	dependson(Dependency)

	local dependencyFolder = _Phantom_include(Dependency, Vars) 
	local dependencyPath = dependencyFolder .. "/" .. Dependency
	
	_Phantom_dbg_print("Dependson " .. Dependency)
	_Phantom_dbg_indent(1)
	Phantom_projects_data_[Dependency]["public"](Vars)
	filter {}

	includedirs { dependencyPath }

	if(Vars["Link"] == "Shared" or Phantom_projects_data_[Name]["exe"]) then
		-- _Phantom_dbg_print("Linking " .. Dependency)
		Phantom_projects_data_[Dependency]["link"](Vars)
		filter {}
		libdirs { Phantom_project_output_path(Dependency, Vars) }
		links { Dependency }
		filter {}
	end

	filter {}

	-- end
	
	_Phantom_dbg_indent(-1)

end

function _Phantom_collect_dependencies(Name, Vars, Result)

	local projectConfig = Phantom_projects_data_[Name]
	if(projectConfig == nil) then
		error("no project '".. Name .."' exist in the solution (forgot a call to 'Phantom_add_project' ?)")
		return
	end
	local dependencies = Phantom_projects_data_[Name]["dependencies"]

	-- recursive
	for i,dependency in ipairs(dependencies) do
		local alreadyAdded = false
		for j,result in ipairs(Result) do
			if(result == dependency) then
				alreadyAdded = true;
				break;
			end
		end
		if(not alreadyAdded) then
			table.insert(Result, dependency)
			_Phantom_collect_dependencies(dependency, Vars, Result)
		end
	end

end

function Phantom_pch()
	pchheader(Phantom_current_project_ .. ".pch.h")
	pchsource(Phantom_project_data("SourcePath") .. Phantom_current_project_ .. ".pch.cpp")
	filter { "action:vs*", "platforms:Win32 or x64" }
		buildoptions { "/FI" .. Phantom_current_project_ .. ".pch.h"}
	filter {}
end

function Phantom_add_project(Name, Vars)
	
	local ProjectFolder = _Phantom_project_folder(Name, Vars)

	if(ProjectFolder == nil) then
		print("'" .. Name .. "' not found, it will not be built")
		return
	end

	_Phantom_dbg_print("=============================================")
	_Phantom_dbg_print(Name)

	_Phantom_dbg_indent(1)

	local usePrecompiledHeaders = true
    	
	-- folder and source path
	
	include(ProjectFolder .. "/" .. Name .. "/" .. Name .. ".premake")
	
	
	local ProjectPath = ProjectFolder .. "/" .. Name .. "/"
	
	Phantom_projects_data_[Name]["SourcePath"] = ProjectPath
	
	local ProjectOutDir = Phantom_project_output_path(Name, Vars)

	local usePrecompiledHeaders = true
		
	project(Name)

		filter {"configurations:Release or Final"}
			runtime "Release"
		filter {"configurations:Debug or DebugOpt"}
			runtime "Debug"
		filter {}
	
		filter {"configurations:Release or Final"}
			runtime "Release"
		filter {"configurations:Debug or DebugOpt"}
			runtime "Debug"
		filter {}
	
		Phantom_current_project_ = Name
		
		Phantom_current_vars_ = Vars

		if(Phantom_projects_data_[Name]["exe"]) then
			kind "ConsoleApp"
		elseif(Vars["Link"] == "Shared") then
			kind "SharedLib"
		else
			kind "StaticLib"
		end 
		
		if(Vars["Link"] == "Static") then
			defines { "PHANTOM_STATIC_LIB" }
		end
		
		objdir("!build/$(SolutionName)/".. _ACTION .. "/Obj/$(Configuration)_$(Platform)_$(ProjectName)")			-- Keep VS evaluating variables (also easier to view/edit in VS)

		flags { "MultiProcessorCompile", "NoMinimalRebuild", "NoBufferSecurityCheck", "NoRuntimeChecks", "FatalWarnings" }
		rtti "On"
		symbols "On"    -- This option disable "editandcontinue" better than [editandcontinue "Off"]

		debugdir "."

		local dependencies = {}
		_Phantom_collect_dependencies(Name, Vars, dependencies)

		-- use public includes + defines from the dependency projects
		for i,dependency in ipairs(dependencies) do
			_Phantom_add_dependency(Name, dependency, Vars)
		end
		
		-- copy dll from lib to bin
		
		local BinPath = ProjectOutDir .. "/../bin"
		local projectPathRelativeToSolution = path.getrelative(Phantom_solution_path(Vars), ProjectOutDir) .. "/"
		local projectPathRelativeToSolution_Win = path.translate(projectPathRelativeToSolution)
		local binPathRelativeToSolution = path.getrelative(Phantom_solution_path(Vars), BinPath) .. "/"
		local binPathRelativeToSolution_Win = path.translate(binPathRelativeToSolution)
		filter { "platforms:Win32 or x64", "kind:SharedLib" }
		postbuildcommands {
			"xcopy /f/y/d/i  \"" ..projectPathRelativeToSolution_Win .. "*.dll\" \"" .. binPathRelativeToSolution_Win .."\"",
			"xcopy /f/y/d/i  \"" ..projectPathRelativeToSolution_Win .. "*.pdb\" \"" .. binPathRelativeToSolution_Win .. "\"",
	        "exit /b 0",
		}

		-- includes / defines
		filter {}

		includedirs {
			ProjectPath 
		}
		
		Phantom_projects_data_[Name]["public"](Vars)
		filter {}
		
		-- files

		files {
			ProjectPath .. "**.c",
			ProjectPath .. "**.cpp",
			ProjectPath .. "**.h",
			ProjectPath .. "**.hxx",
			ProjectPath .. "**.inl",
			ProjectPath .. "**.lua",
		}
		
		if(not Phantom_projects_data_[Name]["exe"]) then
		   filter { "platforms:Win32 or x64", "kind:not SharedLib" }
			postbuildcommands {
			      "xcopy /f/y/d/i \"$(IntDir)\\" .. Name .. ".pdb\" \"$(OutDir)\" ",
			      "exit /b 0",
			}
		   filter {}
		end

		Phantom_projects_data_[Name]["private"](Vars) -- the 'kind' must be defined here

	    filter { "kind:SharedLib" }
	        defines { "DLL_" .. string.gsub(string.upper(Name), "%.", "_") }
	    filter {}

		if(Vars["Link"] == "Shared" or Phantom_projects_data_[Name]["exe"]) then -- dll or exe => we link
			Phantom_projects_data_[Name]["link"](Vars)
		end
	
		filter {}
		
		targetdir(ProjectOutDir)

		language "C++"
		
		filter { "action:vs*", "platforms:Win32 or x64" }
			buildoptions {"/bigobj /vmg /vmv"}
			-- editandcontinue requires IncrementalLink
			if(Vars["Link"] == "Static") then
				flags { "NoIncrementalLink" }
				buildoptions {"/FC"}               -- Use full path (Needed by phantom)
				editandcontinue "Off"
			else
				editandcontinue "On"
			end
		filter { "platforms:Win32 or x64", "configurations:Final"}
			-- Disable some warnings which apear only when ASSERT are disabled
			disablewarnings { "4100" }      -- unreferenced formal parameter
			disablewarnings { "4189" }      -- 'XXX': local variable is initialized but not referenced
		filter {}
		
		if(Vars["Link"] == "Static") then      -- Every static
			linkLibraryDependencies "true"     -- Avoid unused symbols stripping for reflection
			useLibraryDependencyInputs "true"  -- Make the build have a end in static
		end

		vpaths {
			["*"] = ".",
		}

		filter {}
		
	_Phantom_dbg_indent(-1)
end
