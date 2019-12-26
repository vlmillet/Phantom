-- Here is an example of the most common used configuration functions
-- For more in-depth use of premake, refer to the online documentation:
-- https://github.com/premake/premake-core/wiki

Phantom_console_app("HelloWorld",  { "Phantom" } -- DEPENDENCIES
	,
	function(Vars)  -- include private

	end
	,
	function(Vars) -- link
		filter {}
	end

)
