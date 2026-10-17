//-- Mapper overlay mechanism test (MSX2 + mapper + MSX-DOS 2)
ProjName = "maptest";
ProjModules = [ ProjName ];
LibModules = [ "system", "bios", "vdp", "memory", "dos", "dos_mapper", "string" ];
Machine = "2";
Target = "DOS2";
CompileComplexity = "Default";
Verbose = true;
