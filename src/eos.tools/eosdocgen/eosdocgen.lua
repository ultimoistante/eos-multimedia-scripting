-- EOS Documentation Generator
--
-- This code is designed to receive a C source file as input, and extract from
-- it comment structures opened with "/*-" and closed by "*/" tags.
-- It generates standard html output.
--
-- Authors: Salvatore Carotenuto <ultimoistante@gmail.com>
--          Alessandro Rasulo <alex9978@gmail.com>
--
-- v. 0.1 - 24/09/2009
--
-- Usage:
--   lua elfdocgen.lua <inputfile> <outputdir>
--

-- --------------------------------------------------------------------
-- --------------------------------------------------------------------

-- global table to contain functions
eosdocgen = {
            programName = "eosdocgen",
            programVersion = "0.1",
            html = {
                   cssfilename = "eosdocgen.css",
                   }
            }


-- reads a file, returning its content as a string 
function eosdocgen.readFile(inputfile)
  --
  local inputfile = io.open(inputfile)
  if(inputfile ~= nil) then
    local fileContent = inputfile:read("*all")
    inputfile:close()
    return fileContent
  end
  --
  return nil
end 

function trim (s)
  return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

-- reads a multiline string, returning a table containing only comment lines
function eosdocgen.extractCommentLines(inputfile)
  --
  local commentLines = {}
  local inputfile = io.open(inputfile)
  if(inputfile ~= nil) then
    -- read the lines in table 'lines'
    for line in inputfile:lines() do
      line = trim(line)
      -- pattern matching: "*", "*/", "/*-"
      if(line:sub(1, 1) == "*" or line:sub(1, 3) == "/*-") then
        table.insert(commentLines, line)
      end
    end
  end
  return commentLines
end


-- reads the table containing only comment lines, and subdivides them in balanced blocks
function eosdocgen.subdivideCommentBlocks(commentTable)
  --
  if(commentTable ~= nil) then
    commentTable.blocks = {}
    local currentBlock = {}
    for i,line in ipairs(commentTable) do
      if(line:sub(1, 1) == "*" or line:sub(1, 3) == "/*-") then
        if(line:sub(1, 3) == "/*-") then -- start of block
          currentBlock = { balanced = true }
        end
        table.insert(currentBlock, line)
      end
      if(line:sub(1, 2) == "*/" and currentBlock.balanced) then
        table.insert(commentTable.blocks, currentBlock)
      end
    end
  end
  --
  return commentTable
end


-- reads the table containing balanced blocks, and separates function blocks from other blocks
-- returns:
--  moduleBlocks - table cointaining module global blocks
--  functionBlocks - table cointaining function documentation blocks
function eosdocgen.filterCommentBlocks(commentTable)
  --
  local moduleBlocks = {}
  local functionBlocks = {}
  --
  if(commentTable ~= nil) then
    -- iterates blocks
    for i,block in ipairs(commentTable.blocks) do
      -- iterates lines in blocks
      for i,line in ipairs(block) do
        local tagstart, tagend
        tagstart, tagend = line:find("@function")
        if(tagstart ~= nil) then
          table.insert(functionBlocks, block)
        else
          tagstart, tagend = line:find("@module")
          if(tagstart ~= nil) then
            table.insert(moduleBlocks, block)
          end
        end
      end
    end
  end
  --
  return moduleBlocks, functionBlocks
end


-- parses module block table. Returns a table filled with module comment fields
function eosdocgen.parseModuleBlock(moduleBlocks)
  --
  local moduleFields = {}
  --
  if(moduleBlocks ~= nil) then
    -- browses module blocks
    for i,block in ipairs(moduleBlocks) do
      for j,line in ipairs(block) do
        local tagstart, tagend
        --
        -- extracts module name
        tagstart, tagend = line:find("@module")
        if(tagstart ~= nil) then
          moduleFields.name = trim(line:sub(tagend+1, line:len()))
        end
        --
        -- extracts module brief description
        tagstart, tagend = line:find("@brief")
        if(tagstart ~= nil) then
          moduleFields.brief = trim(line:sub(tagend+1, line:len()))
        end
      end
    end
    --
  end
  --
  return moduleFields
end


-- parses functions block table. Returns a table filled with function comment fields
function eosdocgen.parseFunctionBlock(functionblock)
  --
  local functionFields = {}
  --
  if(functionblock ~= nil) then
    -- parses function block
    for i,line in ipairs(functionblock) do
      local tagstart, tagend
      --
      tagstart, tagend = line:find("@function")
      if(tagstart ~= nil) then
        functionFields.name = trim(line:sub(tagend+1, line:len()))
      end
      --
      tagstart, tagend = line:find("@brief")
      if(tagstart ~= nil) then
        functionFields.brief = trim(line:sub(tagend+1, line:len()))
      end
      --
      tagstart, tagend = line:find("@param")
      if(tagstart ~= nil) then
        line = line:sub(tagend+1, line:len())
        if(functionFields.params == nil) then functionFields.params = {} end
        local tokenIndex = 1
        local paramName = ""
        for word in string.gmatch(line, "%a+") do
          if(tokenIndex == 1) then paramName = word; break; end
          tokenIndex = tokenIndex + 1
        end
        tagstart, tagend = line:find(paramName)
        table.insert(functionFields.params, {name=paramName, brief=trim(line:sub(tagend+1, line:len()))} )
      end
      --
      tagstart, tagend = line:find("@return")
      if(tagstart ~= nil) then
        line = line:sub(tagend+1, line:len())
        if(functionFields.returns == nil) then functionFields.returns = {} end
        local tokenIndex = 1
        local returnName = ""
        for word in string.gmatch(line, "%a+") do
          if(tokenIndex == 1) then returnName = word; break; end
          tokenIndex = tokenIndex + 1
        end
        tagstart, tagend = line:find(returnName)
        table.insert(functionFields.returns, {name=returnName, brief=trim(line:sub(tagend+1, line:len()))} )
      end
      --
    end
    --
  end
  --
  return functionFields
end


-- html output: writes file header
function eosdocgen.html.writeHeader(outputfilename)
  --
  local outfile = io.open(outputfilename, "w")
  if(outfile ~= nil) then
    outfile:write(string.format("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"))
    outfile:write(string.format("<html>\n"))
    outfile:write(string.format("\t<head>\n"))
    outfile:write(string.format("\t\t<meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\">\n"))
    outfile:write(string.format("\t\t<title>EOS Module Reference</title>\n"))
    outfile:write(string.format("\t\t<link href=\"%s\" rel=\"stylesheet\" type=\"text/css\">\n", eosdocgen.html.cssfilename))
    outfile:write(string.format("\t</head>\n"))
    outfile:write(string.format("\t<body>\n"))
    outfile:write(string.format("\t\t<!-- Generated by %s %s -->\n", eosdocgen.programName, eosdocgen.programVersion))
    --
    outfile:close()
  end
end


-- html output: writes html footer
function eosdocgen.html.writeFooter(outputfilename)
  --
  local outfile = io.open(outputfilename, "a")
  if(outfile ~= nil) then
    outfile:write(string.format("\t\t<div class=\"page_footer\">\n"))
    outfile:write(string.format("\t\t\t<p class=\"smalltext\">Page generated by %s v.%s, on %s</p>\n", eosdocgen.programName, eosdocgen.programVersion, os.date("%B %d, %Y - %H:%M")))
    outfile:write(string.format("\t\t</div>\n"))
    outfile:write(string.format("\t</body>\n"))
    outfile:write(string.format("</html>\n"))
    outfile:close()
  end
end


-- html output: writes module informations header
function eosdocgen.html.writeModuleHeader(outputfilename, moduleFields)
  --
  if(moduleFields ~= nil) then
    if(moduleFields.name == nil or moduleFields.brief == nil) then
      print("eosdocgen: something wrong in module documentation header. Please check source file.")
      print("  example module header:")
      print("  /*- @module <module.name>")
      print("   *  @brief <module.brief.description>")
      print("   */")
    else
      local outfile = io.open(outputfilename, "a")
      if(outfile ~= nil) then
        outfile:write(string.format("\t\t<div class=\"module_header\">\n"))
        outfile:write(string.format("\t\t\t<p class=\"module_name\">%s</p>\n", moduleFields.name))
        outfile:write(string.format("\t\t\t<p class=\"module_brief\">%s</p>\n", moduleFields.brief))
        outfile:write(string.format("\t\t</div>\n"))
        outfile:close()
      end
    end
  end
end


-- html output: writes functions table header
function eosdocgen.html.openBriefFunctionsTable(outputfilename)
  --
  local outfile = io.open(outputfilename, "a")
  if(outfile ~= nil) then
    outfile:write(string.format("\t\t<div class=\"functions_list\">\n"))
    outfile:write(string.format("\t\t\t<p>Brief functions list:</p>\n"))
    outfile:write(string.format("\t\t\t<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n"))
    outfile:close()
  end
end


-- html output: writes functions table header
function eosdocgen.html.openDetailedFunctionsTable(outputfilename)
  --
  local outfile = io.open(outputfilename, "a")
  if(outfile ~= nil) then
    outfile:write(string.format("\t\t<div class=\"functions\">\n"))
    outfile:write(string.format("\t\t\t<p>Detailed functions list:</p>\n"))
    outfile:write(string.format("\t\t\t<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n"))
    outfile:close()
  end
end


-- html output: writes functions table footer
function eosdocgen.html.closeFunctionsTable(outputfilename)
  --
  local outfile = io.open(outputfilename, "a")
  if(outfile ~= nil) then
    outfile:write(string.format("\t\t\t</table>\n"))
    outfile:write(string.format("\t\t</div>\n"))
    outfile:close()
  end
end


-- html output: writes a function entry in the output file
function eosdocgen.html.writeFunctionBrief(outputfilename, functionCommentFields)
  --
  local outfile = io.open(outputfilename, "a")
  if(outfile ~= nil) then
    if(functionCommentFields.name ~= nil) then
      if(functionCommentFields.brief == nil) then functionCommentFields.brief = "" end
      outfile:write(string.format("\t\t\t\t<tr><td class=\"function_list_left\"><span class=\"function_list-name\">%s</span></td><td class=\"function_list_right\"><span class=\"function_list-brief\">%s</span></td></tr>\n", functionCommentFields.name, functionCommentFields.brief))
    end
    outfile:close()
  end
end


-- html output: writes a function entry in the output file
function eosdocgen.html.writeFunctionDetailed(outputfilename, functionCommentFields)
  --
  local outfile = io.open(outputfilename, "a")
  if(outfile ~= nil) then
    if(functionCommentFields.name ~= nil) then
      outfile:write(string.format("\t\t\t\t<tr><td class=\"function_name_left\">function</td><td class=\"function_name_right\">%s</td></tr>\n", functionCommentFields.name))
    end
    if(functionCommentFields.brief ~= nil) then
      outfile:write(string.format("\t\t\t\t<tr><td class=\"function_brief_left\">&nbsp;</td><td class=\"function_brief_right\"><span class=\"brief\">%s</span></td></tr>\n", functionCommentFields.brief))
    end
    if(functionCommentFields.params ~= nil) then
      outfile:write(string.format("\t\t\t\t<tr><td class=\"function_params_header_left\">&nbsp;</td><td class=\"function_params_header_right\">Parameters:</td></tr>\n"))
      for i,param in ipairs(functionCommentFields.params) do
        outfile:write(string.format("\t\t\t\t<tr><td class=\"function_param_left\">&nbsp;</td><td class=\"function_param_right\"><span class=\"param_name\">%s</span> %s</td></tr>\n", param.name, param.brief))
      end
    end
    if(functionCommentFields.returns ~= nil) then
      outfile:write(string.format("\t\t\t\t<tr><td class=\"function_returns_header_left\">&nbsp;</td><td class=\"function_returns_header_right\">Returns:</td></tr>\n"))
      for i,ret in ipairs(functionCommentFields.returns) do
        outfile:write(string.format("\t\t\t\t<tr><td class=\"function_return_left\">&nbsp;</td><td class=\"function_return_right\"><span class=\"return_name\">%s</span> %s</td></tr>\n", ret.name, ret.brief))
      end
    end
    outfile:close()
  end
end


-- --------------------------------------------------------------------
-- main loop
-- --------------------------------------------------------------------

-- first of all, extracts all comments lines from source file
local comments = eosdocgen.extractCommentLines(arg[1])
-- then subdivides all the comment lines in balanced blocks
comments = eosdocgen.subdivideCommentBlocks(comments)
-- then separates module blocks from functions blocks
local moduleBlocks, functionBlocks = eosdocgen.filterCommentBlocks(comments)

local outfile = "./output.html"

-- writes html main header
eosdocgen.html.writeHeader(outfile)
-- writes module header (main module data)
local moduleFields = eosdocgen.parseModuleBlock(moduleBlocks)
eosdocgen.html.writeModuleHeader(outfile, moduleFields)
-- writes brief functions list table opening
eosdocgen.html.openBriefFunctionsTable(outfile)
-- writes all functions
for i,block in ipairs(functionBlocks) do
  local functionCommentFields = eosdocgen.parseFunctionBlock(block)
  eosdocgen.html.writeFunctionBrief(outfile, functionCommentFields)
end
-- closes functions table
eosdocgen.html.closeFunctionsTable(outfile)
-- writes detailed functions list table opening
eosdocgen.html.openDetailedFunctionsTable(outfile)
-- writes all functions
for i,block in ipairs(functionBlocks) do
  local functionCommentFields = eosdocgen.parseFunctionBlock(block)
  eosdocgen.html.writeFunctionDetailed(outfile, functionCommentFields)
end
-- closes functions table
eosdocgen.html.closeFunctionsTable(outfile)
-- writes document footer
eosdocgen.html.writeFooter(outfile)

