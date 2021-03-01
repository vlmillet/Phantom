-- Here is an example of the most common used configuration functions
-- For more in-depth use of premake, refer to the online documentation:
-- https://github.com/premake/premake-core/wiki
-- Here is an example of the most common used configuration functions
-- For more in-depth use of premake, refer to the online documentation:
-- https://github.com/premake/premake-core/wiki

include "premake/functions"

if(PhantomSolutionName == nil) then
	PhantomSolutionName = "Phantom"
end

solution(PhantomSolutionName)

	local outputPath = "build/".. PhantomSolutionName.."/" .. _ACTION .. "/"
	local outputPathWin = "build\\".. PhantomSolutionName.."\\" .. _ACTION .. "\\"

	file = io.open("Haunt.bat", "w")
	file:write("pushd .\n\n") -- Save the current working directory (Must be the first line)
	file:write("cd " .. outputPathWin .. "\n")
	file:close()
	
	file = io.open("HauntClearCache.bat", "w")
	file:write("pushd .\n\n") -- Save the current working directory (Must be the first line)
	file:write("cd " .. outputPathWin .. "\n")
	file:close()

	platforms { "x64" }
	configurations { "Debug", "DebugOpt", "Release", "Final" }

	startproject "HelloWorld"

	location (outputPath)
	
	local Vars = {}
	
	Vars["HauntOptions"] = ""
	if(_OPTIONS["haunt-options"] ~= nil)
	then
		Vars["HauntOptions"] = _OPTIONS["haunt-options"]
	end

	Vars["ProjectSearchPaths"] = {} 
	table.insert(Vars["ProjectSearchPaths"], "./projects")
	Vars["SolutionName"] =  PhantomSolutionName
	Vars["WorkspacePath"] ="../"
	print("WorkspacePath:" .. Vars["WorkspacePath"])
	Vars["Link"] = "Shared"

	group "Libraries"
	Vars["Link"] = "Shared"
	Phantom_add_project("Phantom", Vars)
	Phantom_add_project("Phantom.Code", Vars)
	Phantom_add_project("Phantom.JIT", Vars)
	Phantom_add_project("Phantom.Serialization", Vars)
	
	group "Tests"
	Vars["Exception"] = true
	Vars["Exception"] = false
	
	group "Examples"
		Phantom_add_project("HelloWorld", Vars) 
		Phantom_add_project("HelloWorld.Code", Vars) 
		Phantom_add_project("HelloWorld.JIT", Vars) 
	
	if PhantomExtraProjects ~= nil then
		PhantomExtraProjects(Vars)
	end

	file = io.open("Haunt.bat", "a")
	file:write("\npopd\n") -- Restore the previous working directory saved by pushd (Must be the last line)
	file:close()
	file = io.open("HauntClearCache.bat", "a")
	file:write("\npopd\n") -- Restore the previous working directory saved by pushd (Must be the last line)
	file:close()



