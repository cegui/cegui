--
-- tolua++ premake script
--

if TOLUA_STATIC then
    cegui_dynamic("tolua++", "c", "lib")
    define("TOLUA_STATIC")
else
    cegui_dynamic("tolua++", "c")
    define("TOLUA_EXPORTS")
end

package.files =
{
	matchfiles(pkgdir.."*.c"),
	matchfiles(pkgdir.."*.h"),
}

-- We need to remove edit-and-continue in VS2003 (7.1) or older versions because 
-- of the __LINE__ expansion macro bug (see http://support.microsoft.com/kb/199057/en)
if target=="vs2002" or target=="vs2003" then
	package.buildflags = { "no-edit-and-continue" }
end

library("lua50", "_d")
