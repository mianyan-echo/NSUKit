/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "NSUKit", "index.html", [
    [ "简介", "index.html", "index" ],
    [ "🚀快速开始", "md_markdown_02__quickstart.html", [
      [ "目录", "md_markdown_02__quickstart.html#autotoc_md7", null ],
      [ "1. 环境安装", "md_markdown_02__quickstart.html#md_环境安装", [
        [ "1.1. 环境依赖", "md_markdown_02__quickstart.html#autotoc_md9", null ],
        [ "1.2. 支持的操作系统与平台", "md_markdown_02__quickstart.html#autotoc_md10", null ],
        [ "1.3. 安装", "md_markdown_02__quickstart.html#autotoc_md11", null ]
      ] ],
      [ "2. 基础接口", "md_markdown_02__quickstart.html#md_基础接口", [
        [ "2.1. 片上系统类NSUSoc", "md_markdown_02__quickstart.html#md_片上系统类NSUSoc", [
          [ "2.1.1. 指定协议接口", "md_markdown_02__quickstart.html#md_指定协议接口", null ],
          [ "2.1.2. 发起连接", "md_markdown_02__quickstart.html#md_发起连接", null ]
        ] ],
        [ "2.2. 基于NSUSoc对象的三种交互", "md_markdown_02__quickstart.html#md_基于NSUSoc对象的三种交互", [
          [ "2.2.1 指令寄存器交互", "md_markdown_02__quickstart.html#md_指令寄存器交互", [
            [ "2.2.1.1. 单地址写入/读取", "md_markdown_02__quickstart.html#autotoc_md13", null ],
            [ "2.2.1.2. 片写入/读取", "md_markdown_02__quickstart.html#autotoc_md14", null ],
            [ "2.2.1.3. 单址片写入/读取", "md_markdown_02__quickstart.html#autotoc_md15", null ]
          ] ],
          [ "2.2.2. 指令流交互", "md_markdown_02__quickstart.html#md_指令流交互", [
            [ "2.2.2.1. 配置参数", "md_markdown_02__quickstart.html#autotoc_md16", null ],
            [ "2.2.2.2. 获取参数", "md_markdown_02__quickstart.html#autotoc_md17", null ],
            [ "2.2.2.3. 执行指令", "md_markdown_02__quickstart.html#autotoc_md18", null ]
          ] ],
          [ "2.2.3. 数据流交互", "md_markdown_02__quickstart.html#md_数据流交互", null ]
        ] ]
      ] ]
    ] ],
    [ "进阶使用", "md_markdown_03__professional.html", [
      [ "目录", "md_markdown_03__professional.html#autotoc_md21", null ],
      [ "基本结构", "md_markdown_03__professional.html#autotoc_md23", null ],
      [ "自闭环仿真", "md_markdown_03__professional.html#autotoc_md25", null ],
      [ "数据流交互", "md_markdown_03__professional.html#autotoc_md27", [
        [ "同步接口", "md_markdown_03__professional.html#autotoc_md28", null ],
        [ "异步方式", "md_markdown_03__professional.html#autotoc_md29", null ]
      ] ]
    ] ],
    [ "ICD文件格式", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html", [
      [ "1. ICD指令交互流程", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md32", [
        [ "1.1. 请求包头", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md33", null ],
        [ "1.2. 反馈包头", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md34", null ]
      ] ],
      [ "2. icd.json格式", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md36", [
        [ "2.1. fpack 参数描述", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md37", null ],
        [ "2.2. pname 参数名", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md38", null ],
        [ "2.3. cname 指令名", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md39", null ],
        [ "2.4. pmap 参数映射", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md40", null ],
        [ "2.5. cmap 指令映射", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md41", null ],
        [ "2.6. param 组", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#md_ICDScheme_param_group", null ],
        [ "2.7. command 组", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#md_ICDScheme_cmd_group", null ],
        [ "2.8. sequence 组", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md42", null ]
      ] ],
      [ "3. icd.json示例", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md44", null ],
      [ "4. icd解析程序", "md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md46", null ]
    ] ],
    [ "自定义协议层", "md_markdown_05__custom_protocol_layer.html", [
      [ "自定义接口", "md_markdown_05__custom_protocol_layer.html#autotoc_md49", [
        [ "自定义指令接口", "md_markdown_05__custom_protocol_layer.html#autotoc_md50", null ],
        [ "自定义数据流接口", "md_markdown_05__custom_protocol_layer.html#autotoc_md51", null ]
      ] ]
    ] ],
    [ "自定义处理层", "md_markdown_06__custom_processing_layer.html", [
      [ "icd_paresr", "md_markdown_06__custom_processing_layer.html#md_icd_paresr", [
        [ "icd.json", "md_markdown_06__custom_processing_layer.html#autotoc_md53", null ],
        [ "config(self, **kwargs):", "md_markdown_06__custom_processing_layer.html#autotoc_md54", null ],
        [ "get_param(self, param_name: str, default=0, fmt_type=int):", "md_markdown_06__custom_processing_layer.html#autotoc_md56", null ],
        [ "set_param(self, param_name: str, value, fmt_type=int):", "md_markdown_06__custom_processing_layer.html#autotoc_md57", null ]
      ] ],
      [ "virtual_chnl", "md_markdown_06__custom_processing_layer.html#autotoc_md59", [
        [ "virtual_chnl未开启时", "md_markdown_06__custom_processing_layer.html#autotoc_md60", null ],
        [ "virtual_chnl开启时", "md_markdown_06__custom_processing_layer.html#autotoc_md61", null ]
      ] ]
    ] ],
    [ "协议详解", "md_markdown_07__protocol_details.html", [
      [ "名词解释", "md_markdown_07__protocol_details.html#autotoc_md70", null ]
    ] ],
    [ "NRD FDK", "md_markdown_app_nrd_fdk.html", [
      [ "1. 指令交互", "md_markdown_app_nrd_fdk.html#autotoc_md73", [
        [ "1.1. ICD列表", "md_markdown_app_nrd_fdk.html#autotoc_md74", null ],
        [ "1.2. 指令交互示例", "md_markdown_app_nrd_fdk.html#autotoc_md75", null ]
      ] ],
      [ "2. 寄存器交互", "md_markdown_app_nrd_fdk.html#autotoc_md77", [
        [ "2.1. 寄存器列表", "md_markdown_app_nrd_fdk.html#autotoc_md78", null ],
        [ "2.2. 指令交互示例", "md_markdown_app_nrd_fdk.html#autotoc_md79", null ]
      ] ],
      [ "3. 数据流交互", "md_markdown_app_nrd_fdk.html#autotoc_md81", null ]
    ] ],
    [ "〽️Oscilloscope mode", "md_markdown_app__oscilloscope_mode.html", [
      [ "1. 设备初始化", "md_markdown_app__oscilloscope_mode.html#autotoc_md84", [
        [ "1.1. 实例化Soc对象，并连接目标Soc", "md_markdown_app__oscilloscope_mode.html#autotoc_md85", null ],
        [ "1.2. 初始化Soc", "md_markdown_app__oscilloscope_mode.html#autotoc_md86", null ]
      ] ],
      [ "2. 触发配置", "md_markdown_app__oscilloscope_mode.html#autotoc_md87", [
        [ "2.1. 配置触发使能及触发电平", "md_markdown_app__oscilloscope_mode.html#autotoc_md88", null ],
        [ "2.2. 配置预取点数", "md_markdown_app__oscilloscope_mode.html#autotoc_md89", null ]
      ] ],
      [ "3. 多通道采集配置", "md_markdown_app__oscilloscope_mode.html#autotoc_md90", [
        [ "3.1. 配置采集总点数", "md_markdown_app__oscilloscope_mode.html#autotoc_md91", null ],
        [ "3.2. Start/Stop", "md_markdown_app__oscilloscope_mode.html#autotoc_md92", null ]
      ] ],
      [ "4. 多通道数据获取", "md_markdown_app__oscilloscope_mode.html#autotoc_md93", [
        [ "4.1. DS交互", "md_markdown_app__oscilloscope_mode.html#autotoc_md94", null ],
        [ "4.2. 多通道数据交织格式", "md_markdown_app__oscilloscope_mode.html#autotoc_md95", null ]
      ] ],
      [ "5. 算力模块调用(可选)", "md_markdown_app__oscilloscope_mode.html#autotoc_md96", [
        [ "5.1. Nvidia AGX GPU", "md_markdown_app__oscilloscope_mode.html#autotoc_md97", null ]
      ] ]
    ] ],
    [ "更新记录", "md_markdown__c_h_a_n_g_e_l_o_g.html", null ],
    [ "README", "md__d___work__cpp_project__n_s_u_kit_examples_cmake_build_release_visual_studio__release__r_e_a_d_m_e.html", null ],
    [ "NSUKit使用用例", "md__d___work__cpp_project__n_s_u_kit_examples__r_e_a_d_m_e.html", [
      [ "使用方法", "md__d___work__cpp_project__n_s_u_kit_examples__r_e_a_d_m_e.html#autotoc_md100", null ],
      [ "示例使用", "md__d___work__cpp_project__n_s_u_kit_examples__r_e_a_d_m_e.html#autotoc_md101", [
        [ "指令控制", "md__d___work__cpp_project__n_s_u_kit_examples__r_e_a_d_m_e.html#autotoc_md102", null ],
        [ "固件更新(发送文件)", "md__d___work__cpp_project__n_s_u_kit_examples__r_e_a_d_m_e.html#autotoc_md103", null ],
        [ "单包数据上行示例", "md__d___work__cpp_project__n_s_u_kit_examples__r_e_a_d_m_e.html#autotoc_md104", null ],
        [ "数据流多线程上行示例", "md__d___work__cpp_project__n_s_u_kit_examples__r_e_a_d_m_e.html#autotoc_md105", null ]
      ] ]
    ] ],
    [ "待办事项列表", "todo.html", null ],
    [ "命名空间", "namespaces.html", [
      [ "命名空间列表", "namespaces.html", "namespaces_dup" ],
      [ "命名空间成员", "namespacemembers.html", [
        [ "全部", "namespacemembers.html", null ],
        [ "函数", "namespacemembers_func.html", null ],
        [ "变量", "namespacemembers_vars.html", null ]
      ] ]
    ] ],
    [ "类", "annotated.html", [
      [ "类列表", "annotated.html", "annotated_dup" ],
      [ "类索引", "classes.html", null ],
      [ "类继承关系", "hierarchy.html", "hierarchy" ],
      [ "类成员", "functions.html", [
        [ "全部", "functions.html", "functions_dup" ],
        [ "函数", "functions_func.html", "functions_func" ],
        [ "变量", "functions_vars.html", null ],
        [ "类型定义", "functions_type.html", null ],
        [ "枚举", "functions_enum.html", null ],
        [ "相关函数", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "文件", "files.html", [
      [ "文件列表", "files.html", "files_dup" ],
      [ "文件成员", "globals.html", [
        [ "全部", "globals.html", "globals_dup" ],
        [ "函数", "globals_func.html", null ],
        [ "变量", "globals_vars.html", null ],
        [ "类型定义", "globals_type.html", null ],
        [ "枚举", "globals_enum.html", null ],
        [ "宏定义", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"_a_d_trigger_8cpp.html",
"classnsukit_1_1_i___base_cmd_u_itf.html#a8c20ca24455b39587e317b90783edc31",
"classnsukit_1_1_i_c_d_reg_mw.html#a89a6531daa1aba4bee4f6fd2d4a2d512",
"classnsukit_1_1_n_s_u_soc.html#afd231aa09db55ef5b1bca37013146452",
"classnsukit_1_1_sim_stream_u_itf.html#aa0b2e9aa55902c230afbc184b94d8023",
"classnsukit_1_1_u___interface.html#a793a0abbfdd089a7ef7315faa53920bd",
"examples_2cmake-build-release-visual-studio_2_release_2include_2base_2type_8h.html#a324a8aa029ee38ccd1e011c882c0abc7a599a0acf1ea431bd9b22ea92c2a37c97",
"globals_u.html",
"md__i_c_d_xE6_x96_x87_xE4_xBB_xB6_xE6_xA0_xBC_xE5_xBC_x8F.html#autotoc_md42"
];

var SYNCONMSG = '点击 关闭 面板同步';
var SYNCOFFMSG = '点击 开启 面板同步';