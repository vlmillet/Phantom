-- extend.lua

require("vstudio")

-- PREMAKE EXTENSIONS

local	p = premake
local	config = p.config
local   project = p.project
local	api = p.api
local	vstudio = p.vstudio
local	m = vstudio.vc2010

-- Program Database File Name customization
	
local function programDatabaseFile(cfg)
	if cfg.programdatabasefile then
		_p(3,'<ProgramDataBaseFileName>%s</ProgramDataBaseFileName>', cfg.programdatabasefile)
	end
end

local function useLibraryDependencyInputs(format)
	if format.uselibrarydependencyinputs then
		_p(2,'<ProjectReference>')
		_p(3,'<UseLibraryDependencyInputs>%s</UseLibraryDependencyInputs>', format.uselibrarydependencyinputs)
		_p(2,'</ProjectReference>')
	end
end

premake.override(m.elements, "clCompile", function(base, cfg)
	local elements = base(cfg)
	
	if (cfg.platform ~= 'nx') then
		elements = table.join(elements, {
			programDatabaseFile,
			--@SeavenPascal already added elsewhere
			--debugInformationFormat,
		})
	end
	
	return elements
end)

premake.override(m.elements, "itemDefinitionGroup", function(base, cfg)
	local elements = base(cfg)
	elements = table.join(elements, {
		useLibraryDependencyInputs,
	})
	return elements
end)

api.register({
	name = "programdatabasefile",
	scope = "config",
	kind = "string",
})

m.categories.appxmanifest = 
{
	name = "appxmanifest",
	priority = 5,
	extensions = ".appxmanifest",
	
	emitFiles = function(prj, group)
		m.emitFiles(prj, group, "AppxManifest", {m.generatedFile})
	end,

	emitFilter = function(prj, group)
		m.filterGroup(prj, group, "AppxManifest")
	end	
}

m.categories.Image = 
{
	name = "Image",
	priority = 5,
	extensions = ".png",
	
	emitFiles = function(prj, group)
		m.emitFiles(prj, group, "Image", {m.generatedFile})
	end,

	emitFilter = function(prj, group)
		m.filterGroup(prj, group, "Image")
	end	
}

api.register({
	name = "linkLibraryDependencies",
	scope = "config",
	kind = "string",
})

api.register({
	name = "useLibraryDependencyInputs",
	scope = "config",
	kind = "string",
})

---
-- PSSL group
---
m.categories.PSSL = {
	name       = "PSSL",
	extensions = { ".pssl" },
	priority   = 1,

	emitFiles = function(prj, group)
		m.emitFiles(prj, group, "PSSL", {m.generatedFile})
	end,

	emitFilter = function(prj, group)
		m.filterGroup(prj, group, "PSSL")
	end
}

---
-- Natvis group
---
m.categories.Natvis = {
	name       = "Natvis",
	extensions = { ".natvis" },
	priority   = 1,

	emitFiles = function(prj, group)
		m.emitFiles(prj, group, "Natvis", {m.generatedFile})
	end,

	emitFilter = function(prj, group)
		m.filterGroup(prj, group, "Natvis")
	end
}

-- Added Flag to merge user file with project file

api.addAllowed("flags", {"MergeUserConfig","WinRT","NoWinRT"})

premake.override(m, "generateUser", function(base, prj)
	if not prj.flags.MergeUserConfig then
		base(prj);
	end
end)

premake.override(m.elements, "configurationProperties", function(base, cfg)
	local elements = base(cfg)
	if cfg.flags.MergeUserConfig then
		elements = table.join(elements, m.elements.user(cfg))
	end
	return elements
end)

premake.override(path, "getrelative", function(base, src, dst)
	-- normalize the two paths
	src = path.getabsolute(src)
	dst = path.getabsolute(dst)

	-- same directory?
	if (src == dst) then
		return "."
	end
	
	-- dollar macro? Can't tell what the real path is; use absolute
	-- This enables paths like $(SDK_ROOT)/include to work correctly.
	if dst:startswith("$") then
		return dst
	end
	
	src = src .. "/"
	dst = dst .. "/"

	-- find the common leading directories
	local idx = 0
	while (true) do
		local tst = src:find("/", idx + 1, true)
		if tst then
			if src:sub(1,tst) == dst:sub(1,tst) then
				idx = tst
			else
				break
			end
		else
			break
		end
	end
	
	-- if they have nothing in common return absolute path
	if idx == 0 then
		return dst:sub(1, -2)
	end
	
	-- trim off the common directories from the front 
	src = src:sub(idx + 1)
	dst = dst:sub(idx + 1)
	
	-- back up from dst to get to this common parent
	local result = ""		
	idx = src:find("/")
	while (idx) do
		result = result .. "../"
		idx = src:find("/", idx + 1)
	end

	-- tack on the path down to the dst from here
	result = result .. dst

	-- remove the trailing slash
	return result:sub(1, -2)
end)

-- Additional linker optimization flags
api.addAllowed("flags", {"NoReferences", "NoCOMDATFolding", "IncrementalLink"})

-- @SeavenPascal As a note. The flag "IncrementalLink" can 'force' incremental link on optimized builds
premake.override(config, "canLinkIncremental", function(base, cfg)
	if (not(cfg.kind == "StaticLib") and cfg.flags.IncrementalLink) then
		return true
	else
		return base(cfg)
	end
end)

premake.override(m, "optimizeReferences", function(base, cfg)
	-- @SeavenPascal COMDATFolding and OptimizeReferences will generate warnings and be disabled on non optimized builds
	-- Directly use base function if not optimized
	if (config.isOptimizedBuild(cfg) and (cfg.flags.NoCOMDATFolding or cfg.flags.NoReferences)) then
		if (cfg.flags.NoCOMDATFolding) then
			m.element("EnableCOMDATFolding", nil, "false")
		else
			m.element("EnableCOMDATFolding", nil, "true")
		end
		
		if (cfg.flags.NoReferences) then
			m.element("OptimizeReferences", nil, "false")
		else
			m.element("OptimizeReferences", nil, "true")
		end
	else
		base(cfg)
	end
	
	-- @SeavenPascal Old code. Not very clear and can lead to warnings
	--if ((cfg.flags.NoCOMDATFolding or not config.isOptimizedBuild(cfg)) and (cfg.platform ~= "Durango")) then
	--if (cfg.flags.NoCOMDATFolding or not config.isOptimizedBuild(cfg)) then
	--	m.element("EnableCOMDATFolding", nil, "false")
	--else
	--	m.element("EnableCOMDATFolding", nil, "true")
	--end

	--if ((cfg.flags.NoReferences or not config.isOptimizedBuild(cfg)) and (cfg.platform ~= "Durango")) then
	--if (cfg.flags.NoReferences or not config.isOptimizedBuild(cfg)) then
	--	m.element("OptimizeReferences", nil, "false")
	--else
	--	m.element("OptimizeReferences", nil, "true")
	--end
end)

premake.override(m, "debugInformationFormat", function(base, cfg)
	local value
	local tool, toolVersion = p.config.toolset(cfg)
	if (cfg.symbols == p.ON) or (cfg.symbols == "FastLink") then
		if cfg.debugformat == "c7" then
			value = "OldStyle"
		elseif (cfg.editandcontinue == p.OFF) then
			value = "ProgramDatabase"
		else
			value = "EditAndContinue"
		end

		m.element("DebugInformationFormat", nil, value)
	elseif cfg.symbols == p.OFF then
		m.element("DebugInformationFormat", nil, "None")
	end
end)
