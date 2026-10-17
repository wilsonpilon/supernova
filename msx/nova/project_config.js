//-- Supernova MSX2 port
ProjName = "nova";
ProjModules = [ "main", "../game/globals", "../game/util", "../game/game", "../game/rt_msx", "../data/vocab_data",
	"../txt", "../textpool", "gen/ovl_gen", "gen/ovl_stubs" ];
LibModules = [ "system", "bios", "vdp", "memory", "input", "dos", "dos_mapper", "string" ];
Machine = "2";
Target = "DOS2";
Optim = "Size";
CompileComplexity = "Default";
Verbose = true;
