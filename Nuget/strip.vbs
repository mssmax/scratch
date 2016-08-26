rem on error resume next
set xmlObj = CreateObject("MSXML2.DOMDocument")

xmlObj.load wscript.arguments(0)

set doom = xmlObj.SelectSingleNode("*//dependencies")

if not doom is nothing then
doom.ParentNode.RemoveChild doom
end if

set doom = xmlObj.SelectSingleNode("*//licenseUrl")
if not doom is nothing then
doom.ParentNode.RemoveChild doom
end if

set doom = xmlObj.SelectSingleNode("*//projectUrl")
if not doom is nothing then
doom.ParentNode.RemoveChild doom
end if

set doom = xmlObj.SelectSingleNode("*//iconUrl")
if not doom is nothing then
doom.ParentNode.RemoveChild doom
end if

set doom = xmlObj.SelectSingleNode("*//requireLicenseAcceptance")
if not doom is nothing then
doom.ParentNode.RemoveChild doom
end if

set doom = xmlObj.SelectSingleNode("*//tags")
if not doom is nothing then
doom.ParentNode.RemoveChild doom
end if

xmlObj.save wscript.arguments(0)