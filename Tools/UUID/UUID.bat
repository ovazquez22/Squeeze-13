path "c:\windows\system32\wbem" 
start wmic /OUTPUT:CLIPBOARD path win32_computersystemproduct get uuid 
@echo off
