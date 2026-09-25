
adtf_add_filter(resim_replay_filter resim_replay_filter.h stdafx.h stdafx.cpp resim_replay_filter.cpp)
adtf_install_plugin(resim_replay_filter C:\bmw\ADTF_DEV\ReSimulation_ADTF_Project_v0_1\release\modules\resim_replay\bin)

adtf_set_folder(resim_replay_filter C:\bmw\ADTF_DEV\ReSimulation_ADTF_Project_v0_1\ReSimulation_v0.1.0\Replay_Filter\resim_replay_filter)

install(FILES resim_replay_filter.plb.plugindescription DESTINATION C:\bmw\ADTF_DEV\ReSimulation_ADTF_Project_v0_1\debug\modules\resim_replay\bin CONFIGURATIONS Debug)
install(FILES resim_replay_filter.plb.plugindescription DESTINATION C:\bmw\ADTF_DEV\ReSimulation_ADTF_Project_v0_1\release\modules\resim_replay\bin CONFIGURATIONS Release RelWithDebInfo)
