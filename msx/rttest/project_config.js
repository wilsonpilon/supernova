//-- Supernova MSX2 port: runtime test (screen, windows, scrolling, keys, sound)
ProjName = "rttest";
ProjModules = [ ProjName, "../game/rt_msx", "../txt", "../textpool" ];
LibModules = [ "system", "bios", "vdp", "memory", "input", "dos", "dos_mapper", "string" ];
Machine = "2";
Target = "DOS2";
CompileComplexity = "Default";
Verbose = true;
