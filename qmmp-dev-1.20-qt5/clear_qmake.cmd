for  /r %%B in (*.qm *.dll *.a *.Release Makefile* *resource.rc *.Debug *.exe object_script.* *.pc *.prl)  do del /s /q %%B
for /d /r %%B in (.build debug release)  do rmdir /s /q %%B
rmdir /s /q bin\plugins

