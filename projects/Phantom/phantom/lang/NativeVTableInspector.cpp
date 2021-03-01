// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "NativeVTableInspector.h"

namespace phantom
{
namespace lang
{
size_t NativeVTableInspector::sm_ResultIndex = ~size_t(0);

struct VTableInspectorGeneric_P
{
#if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
    static void NativeVTableInspector_setResultIndex(size_t value)
    {
        NativeVTableInspector::sm_ResultIndex = value;
    }
#endif
    void hack_member_function_0_0(void*)
    {
        NativeVTableInspector_setResultIndex(0);
    }
    void hack_member_function_0_1(void*)
    {
        NativeVTableInspector_setResultIndex(1);
    }
    void hack_member_function_0_2(void*)
    {
        NativeVTableInspector_setResultIndex(2);
    }
    void hack_member_function_0_3(void*)
    {
        NativeVTableInspector_setResultIndex(3);
    }
    void hack_member_function_0_4(void*)
    {
        NativeVTableInspector_setResultIndex(4);
    }
    void hack_member_function_0_5(void*)
    {
        NativeVTableInspector_setResultIndex(5);
    }
    void hack_member_function_0_6(void*)
    {
        NativeVTableInspector_setResultIndex(6);
    }
    void hack_member_function_0_7(void*)
    {
        NativeVTableInspector_setResultIndex(7);
    }
    void hack_member_function_0_8(void*)
    {
        NativeVTableInspector_setResultIndex(8);
    }
    void hack_member_function_0_9(void*)
    {
        NativeVTableInspector_setResultIndex(9);
    }
    void hack_member_function_0_10(void*)
    {
        NativeVTableInspector_setResultIndex(10);
    }
    void hack_member_function_0_11(void*)
    {
        NativeVTableInspector_setResultIndex(11);
    }
    void hack_member_function_0_12(void*)
    {
        NativeVTableInspector_setResultIndex(12);
    }
    void hack_member_function_0_13(void*)
    {
        NativeVTableInspector_setResultIndex(13);
    }
    void hack_member_function_0_14(void*)
    {
        NativeVTableInspector_setResultIndex(14);
    }
    void hack_member_function_0_15(void*)
    {
        NativeVTableInspector_setResultIndex(15);
    }
    void hack_member_function_0_16(void*)
    {
        NativeVTableInspector_setResultIndex(16);
    }
    void hack_member_function_0_17(void*)
    {
        NativeVTableInspector_setResultIndex(17);
    }
    void hack_member_function_0_18(void*)
    {
        NativeVTableInspector_setResultIndex(18);
    }
    void hack_member_function_0_19(void*)
    {
        NativeVTableInspector_setResultIndex(19);
    }
    void hack_member_function_0_20(void*)
    {
        NativeVTableInspector_setResultIndex(20);
    }
    void hack_member_function_0_21(void*)
    {
        NativeVTableInspector_setResultIndex(21);
    }
    void hack_member_function_0_22(void*)
    {
        NativeVTableInspector_setResultIndex(22);
    }
    void hack_member_function_0_23(void*)
    {
        NativeVTableInspector_setResultIndex(23);
    }
    void hack_member_function_0_24(void*)
    {
        NativeVTableInspector_setResultIndex(24);
    }
    void hack_member_function_0_25(void*)
    {
        NativeVTableInspector_setResultIndex(25);
    }
    void hack_member_function_0_26(void*)
    {
        NativeVTableInspector_setResultIndex(26);
    }
    void hack_member_function_0_27(void*)
    {
        NativeVTableInspector_setResultIndex(27);
    }
    void hack_member_function_0_28(void*)
    {
        NativeVTableInspector_setResultIndex(28);
    }
    void hack_member_function_0_29(void*)
    {
        NativeVTableInspector_setResultIndex(29);
    }
    void hack_member_function_0_30(void*)
    {
        NativeVTableInspector_setResultIndex(30);
    }
    void hack_member_function_0_31(void*)
    {
        NativeVTableInspector_setResultIndex(31);
    }
    void hack_member_function_0_32(void*)
    {
        NativeVTableInspector_setResultIndex(32);
    }
    void hack_member_function_0_33(void*)
    {
        NativeVTableInspector_setResultIndex(33);
    }
    void hack_member_function_0_34(void*)
    {
        NativeVTableInspector_setResultIndex(34);
    }
    void hack_member_function_0_35(void*)
    {
        NativeVTableInspector_setResultIndex(35);
    }
    void hack_member_function_0_36(void*)
    {
        NativeVTableInspector_setResultIndex(36);
    }
    void hack_member_function_0_37(void*)
    {
        NativeVTableInspector_setResultIndex(37);
    }
    void hack_member_function_0_38(void*)
    {
        NativeVTableInspector_setResultIndex(38);
    }
    void hack_member_function_0_39(void*)
    {
        NativeVTableInspector_setResultIndex(39);
    }
    void hack_member_function_0_40(void*)
    {
        NativeVTableInspector_setResultIndex(40);
    }
    void hack_member_function_0_41(void*)
    {
        NativeVTableInspector_setResultIndex(41);
    }
    void hack_member_function_0_42(void*)
    {
        NativeVTableInspector_setResultIndex(42);
    }
    void hack_member_function_0_43(void*)
    {
        NativeVTableInspector_setResultIndex(43);
    }
    void hack_member_function_0_44(void*)
    {
        NativeVTableInspector_setResultIndex(44);
    }
    void hack_member_function_0_45(void*)
    {
        NativeVTableInspector_setResultIndex(45);
    }
    void hack_member_function_0_46(void*)
    {
        NativeVTableInspector_setResultIndex(46);
    }
    void hack_member_function_0_47(void*)
    {
        NativeVTableInspector_setResultIndex(47);
    }
    void hack_member_function_0_48(void*)
    {
        NativeVTableInspector_setResultIndex(48);
    }
    void hack_member_function_0_49(void*)
    {
        NativeVTableInspector_setResultIndex(49);
    }
    void hack_member_function_0_50(void*)
    {
        NativeVTableInspector_setResultIndex(50);
    }
    void hack_member_function_0_51(void*)
    {
        NativeVTableInspector_setResultIndex(51);
    }
    void hack_member_function_0_52(void*)
    {
        NativeVTableInspector_setResultIndex(52);
    }
    void hack_member_function_0_53(void*)
    {
        NativeVTableInspector_setResultIndex(53);
    }
    void hack_member_function_0_54(void*)
    {
        NativeVTableInspector_setResultIndex(54);
    }
    void hack_member_function_0_55(void*)
    {
        NativeVTableInspector_setResultIndex(55);
    }
    void hack_member_function_0_56(void*)
    {
        NativeVTableInspector_setResultIndex(56);
    }
    void hack_member_function_0_57(void*)
    {
        NativeVTableInspector_setResultIndex(57);
    }
    void hack_member_function_0_58(void*)
    {
        NativeVTableInspector_setResultIndex(58);
    }
    void hack_member_function_0_59(void*)
    {
        NativeVTableInspector_setResultIndex(59);
    }
    void hack_member_function_0_60(void*)
    {
        NativeVTableInspector_setResultIndex(60);
    }
    void hack_member_function_0_61(void*)
    {
        NativeVTableInspector_setResultIndex(61);
    }
    void hack_member_function_0_62(void*)
    {
        NativeVTableInspector_setResultIndex(62);
    }
    void hack_member_function_0_63(void*)
    {
        NativeVTableInspector_setResultIndex(63);
    }
    void hack_member_function_0_64(void*)
    {
        NativeVTableInspector_setResultIndex(64);
    }
    void hack_member_function_0_65(void*)
    {
        NativeVTableInspector_setResultIndex(65);
    }
    void hack_member_function_0_66(void*)
    {
        NativeVTableInspector_setResultIndex(66);
    }
    void hack_member_function_0_67(void*)
    {
        NativeVTableInspector_setResultIndex(67);
    }
    void hack_member_function_0_68(void*)
    {
        NativeVTableInspector_setResultIndex(68);
    }
    void hack_member_function_0_69(void*)
    {
        NativeVTableInspector_setResultIndex(69);
    }
    void hack_member_function_0_70(void*)
    {
        NativeVTableInspector_setResultIndex(70);
    }
    void hack_member_function_0_71(void*)
    {
        NativeVTableInspector_setResultIndex(71);
    }
    void hack_member_function_0_72(void*)
    {
        NativeVTableInspector_setResultIndex(72);
    }
    void hack_member_function_0_73(void*)
    {
        NativeVTableInspector_setResultIndex(73);
    }
    void hack_member_function_0_74(void*)
    {
        NativeVTableInspector_setResultIndex(74);
    }
    void hack_member_function_0_75(void*)
    {
        NativeVTableInspector_setResultIndex(75);
    }
    void hack_member_function_0_76(void*)
    {
        NativeVTableInspector_setResultIndex(76);
    }
    void hack_member_function_0_77(void*)
    {
        NativeVTableInspector_setResultIndex(77);
    }
    void hack_member_function_0_78(void*)
    {
        NativeVTableInspector_setResultIndex(78);
    }
    void hack_member_function_0_79(void*)
    {
        NativeVTableInspector_setResultIndex(79);
    }
    void hack_member_function_0_80(void*)
    {
        NativeVTableInspector_setResultIndex(80);
    }
    void hack_member_function_0_81(void*)
    {
        NativeVTableInspector_setResultIndex(81);
    }
    void hack_member_function_0_82(void*)
    {
        NativeVTableInspector_setResultIndex(82);
    }
    void hack_member_function_0_83(void*)
    {
        NativeVTableInspector_setResultIndex(83);
    }
    void hack_member_function_0_84(void*)
    {
        NativeVTableInspector_setResultIndex(84);
    }
    void hack_member_function_0_85(void*)
    {
        NativeVTableInspector_setResultIndex(85);
    }
    void hack_member_function_0_86(void*)
    {
        NativeVTableInspector_setResultIndex(86);
    }
    void hack_member_function_0_87(void*)
    {
        NativeVTableInspector_setResultIndex(87);
    }
    void hack_member_function_0_88(void*)
    {
        NativeVTableInspector_setResultIndex(88);
    }
    void hack_member_function_0_89(void*)
    {
        NativeVTableInspector_setResultIndex(89);
    }
    void hack_member_function_0_90(void*)
    {
        NativeVTableInspector_setResultIndex(90);
    }
    void hack_member_function_0_91(void*)
    {
        NativeVTableInspector_setResultIndex(91);
    }
    void hack_member_function_0_92(void*)
    {
        NativeVTableInspector_setResultIndex(92);
    }
    void hack_member_function_0_93(void*)
    {
        NativeVTableInspector_setResultIndex(93);
    }
    void hack_member_function_0_94(void*)
    {
        NativeVTableInspector_setResultIndex(94);
    }
    void hack_member_function_0_95(void*)
    {
        NativeVTableInspector_setResultIndex(95);
    }
    void hack_member_function_0_96(void*)
    {
        NativeVTableInspector_setResultIndex(96);
    }
    void hack_member_function_0_97(void*)
    {
        NativeVTableInspector_setResultIndex(97);
    }
    void hack_member_function_0_98(void*)
    {
        NativeVTableInspector_setResultIndex(98);
    }
    void hack_member_function_0_99(void*)
    {
        NativeVTableInspector_setResultIndex(99);
    }
    void hack_member_function_0_100(void*)
    {
        NativeVTableInspector_setResultIndex(100);
    }
    void hack_member_function_0_101(void*)
    {
        NativeVTableInspector_setResultIndex(101);
    }
    void hack_member_function_0_102(void*)
    {
        NativeVTableInspector_setResultIndex(102);
    }
    void hack_member_function_0_103(void*)
    {
        NativeVTableInspector_setResultIndex(103);
    }
    void hack_member_function_0_104(void*)
    {
        NativeVTableInspector_setResultIndex(104);
    }
    void hack_member_function_0_105(void*)
    {
        NativeVTableInspector_setResultIndex(105);
    }
    void hack_member_function_0_106(void*)
    {
        NativeVTableInspector_setResultIndex(106);
    }
    void hack_member_function_0_107(void*)
    {
        NativeVTableInspector_setResultIndex(107);
    }
    void hack_member_function_0_108(void*)
    {
        NativeVTableInspector_setResultIndex(108);
    }
    void hack_member_function_0_109(void*)
    {
        NativeVTableInspector_setResultIndex(109);
    }
    void hack_member_function_0_110(void*)
    {
        NativeVTableInspector_setResultIndex(110);
    }
    void hack_member_function_0_111(void*)
    {
        NativeVTableInspector_setResultIndex(111);
    }
    void hack_member_function_0_112(void*)
    {
        NativeVTableInspector_setResultIndex(112);
    }
    void hack_member_function_0_113(void*)
    {
        NativeVTableInspector_setResultIndex(113);
    }
    void hack_member_function_0_114(void*)
    {
        NativeVTableInspector_setResultIndex(114);
    }
    void hack_member_function_0_115(void*)
    {
        NativeVTableInspector_setResultIndex(115);
    }
    void hack_member_function_0_116(void*)
    {
        NativeVTableInspector_setResultIndex(116);
    }
    void hack_member_function_0_117(void*)
    {
        NativeVTableInspector_setResultIndex(117);
    }
    void hack_member_function_0_118(void*)
    {
        NativeVTableInspector_setResultIndex(118);
    }
    void hack_member_function_0_119(void*)
    {
        NativeVTableInspector_setResultIndex(119);
    }
    void hack_member_function_0_120(void*)
    {
        NativeVTableInspector_setResultIndex(120);
    }
    void hack_member_function_0_121(void*)
    {
        NativeVTableInspector_setResultIndex(121);
    }
    void hack_member_function_0_122(void*)
    {
        NativeVTableInspector_setResultIndex(122);
    }
    void hack_member_function_0_123(void*)
    {
        NativeVTableInspector_setResultIndex(123);
    }
    void hack_member_function_0_124(void*)
    {
        NativeVTableInspector_setResultIndex(124);
    }
    void hack_member_function_0_125(void*)
    {
        NativeVTableInspector_setResultIndex(125);
    }
    void hack_member_function_0_126(void*)
    {
        NativeVTableInspector_setResultIndex(126);
    }
    void hack_member_function_0_127(void*)
    {
        NativeVTableInspector_setResultIndex(127);
    }
    void hack_member_function_0_128(void*)
    {
        NativeVTableInspector_setResultIndex(128);
    }
    void hack_member_function_0_129(void*)
    {
        NativeVTableInspector_setResultIndex(129);
    }
    void hack_member_function_0_130(void*)
    {
        NativeVTableInspector_setResultIndex(130);
    }
    void hack_member_function_0_131(void*)
    {
        NativeVTableInspector_setResultIndex(131);
    }
    void hack_member_function_0_132(void*)
    {
        NativeVTableInspector_setResultIndex(132);
    }
    void hack_member_function_0_133(void*)
    {
        NativeVTableInspector_setResultIndex(133);
    }
    void hack_member_function_0_134(void*)
    {
        NativeVTableInspector_setResultIndex(134);
    }
    void hack_member_function_0_135(void*)
    {
        NativeVTableInspector_setResultIndex(135);
    }
    void hack_member_function_0_136(void*)
    {
        NativeVTableInspector_setResultIndex(136);
    }
    void hack_member_function_0_137(void*)
    {
        NativeVTableInspector_setResultIndex(137);
    }
    void hack_member_function_0_138(void*)
    {
        NativeVTableInspector_setResultIndex(138);
    }
    void hack_member_function_0_139(void*)
    {
        NativeVTableInspector_setResultIndex(139);
    }
    void hack_member_function_0_140(void*)
    {
        NativeVTableInspector_setResultIndex(140);
    }
    void hack_member_function_0_141(void*)
    {
        NativeVTableInspector_setResultIndex(141);
    }
    void hack_member_function_0_142(void*)
    {
        NativeVTableInspector_setResultIndex(142);
    }
    void hack_member_function_0_143(void*)
    {
        NativeVTableInspector_setResultIndex(143);
    }
    void hack_member_function_0_144(void*)
    {
        NativeVTableInspector_setResultIndex(144);
    }
    void hack_member_function_0_145(void*)
    {
        NativeVTableInspector_setResultIndex(145);
    }
    void hack_member_function_0_146(void*)
    {
        NativeVTableInspector_setResultIndex(146);
    }
    void hack_member_function_0_147(void*)
    {
        NativeVTableInspector_setResultIndex(147);
    }
    void hack_member_function_0_148(void*)
    {
        NativeVTableInspector_setResultIndex(148);
    }
    void hack_member_function_0_149(void*)
    {
        NativeVTableInspector_setResultIndex(149);
    }
    void hack_member_function_0_150(void*)
    {
        NativeVTableInspector_setResultIndex(150);
    }
    void hack_member_function_0_151(void*)
    {
        NativeVTableInspector_setResultIndex(151);
    }
    void hack_member_function_0_152(void*)
    {
        NativeVTableInspector_setResultIndex(152);
    }
    void hack_member_function_0_153(void*)
    {
        NativeVTableInspector_setResultIndex(153);
    }
    void hack_member_function_0_154(void*)
    {
        NativeVTableInspector_setResultIndex(154);
    }
    void hack_member_function_0_155(void*)
    {
        NativeVTableInspector_setResultIndex(155);
    }
    void hack_member_function_0_156(void*)
    {
        NativeVTableInspector_setResultIndex(156);
    }
    void hack_member_function_0_157(void*)
    {
        NativeVTableInspector_setResultIndex(157);
    }
    void hack_member_function_0_158(void*)
    {
        NativeVTableInspector_setResultIndex(158);
    }
    void hack_member_function_0_159(void*)
    {
        NativeVTableInspector_setResultIndex(159);
    }
    void hack_member_function_0_160(void*)
    {
        NativeVTableInspector_setResultIndex(160);
    }
    void hack_member_function_0_161(void*)
    {
        NativeVTableInspector_setResultIndex(161);
    }
    void hack_member_function_0_162(void*)
    {
        NativeVTableInspector_setResultIndex(162);
    }
    void hack_member_function_0_163(void*)
    {
        NativeVTableInspector_setResultIndex(163);
    }
    void hack_member_function_0_164(void*)
    {
        NativeVTableInspector_setResultIndex(164);
    }
    void hack_member_function_0_165(void*)
    {
        NativeVTableInspector_setResultIndex(165);
    }
    void hack_member_function_0_166(void*)
    {
        NativeVTableInspector_setResultIndex(166);
    }
    void hack_member_function_0_167(void*)
    {
        NativeVTableInspector_setResultIndex(167);
    }
    void hack_member_function_0_168(void*)
    {
        NativeVTableInspector_setResultIndex(168);
    }
    void hack_member_function_0_169(void*)
    {
        NativeVTableInspector_setResultIndex(169);
    }
    void hack_member_function_0_170(void*)
    {
        NativeVTableInspector_setResultIndex(170);
    }
    void hack_member_function_0_171(void*)
    {
        NativeVTableInspector_setResultIndex(171);
    }
    void hack_member_function_0_172(void*)
    {
        NativeVTableInspector_setResultIndex(172);
    }
    void hack_member_function_0_173(void*)
    {
        NativeVTableInspector_setResultIndex(173);
    }
    void hack_member_function_0_174(void*)
    {
        NativeVTableInspector_setResultIndex(174);
    }
    void hack_member_function_0_175(void*)
    {
        NativeVTableInspector_setResultIndex(175);
    }
    void hack_member_function_0_176(void*)
    {
        NativeVTableInspector_setResultIndex(176);
    }
    void hack_member_function_0_177(void*)
    {
        NativeVTableInspector_setResultIndex(177);
    }
    void hack_member_function_0_178(void*)
    {
        NativeVTableInspector_setResultIndex(178);
    }
    void hack_member_function_0_179(void*)
    {
        NativeVTableInspector_setResultIndex(179);
    }
    void hack_member_function_0_180(void*)
    {
        NativeVTableInspector_setResultIndex(180);
    }
    void hack_member_function_0_181(void*)
    {
        NativeVTableInspector_setResultIndex(181);
    }
    void hack_member_function_0_182(void*)
    {
        NativeVTableInspector_setResultIndex(182);
    }
    void hack_member_function_0_183(void*)
    {
        NativeVTableInspector_setResultIndex(183);
    }
    void hack_member_function_0_184(void*)
    {
        NativeVTableInspector_setResultIndex(184);
    }
    void hack_member_function_0_185(void*)
    {
        NativeVTableInspector_setResultIndex(185);
    }
    void hack_member_function_0_186(void*)
    {
        NativeVTableInspector_setResultIndex(186);
    }
    void hack_member_function_0_187(void*)
    {
        NativeVTableInspector_setResultIndex(187);
    }
    void hack_member_function_0_188(void*)
    {
        NativeVTableInspector_setResultIndex(188);
    }
    void hack_member_function_0_189(void*)
    {
        NativeVTableInspector_setResultIndex(189);
    }
    void hack_member_function_0_190(void*)
    {
        NativeVTableInspector_setResultIndex(190);
    }
    void hack_member_function_0_191(void*)
    {
        NativeVTableInspector_setResultIndex(191);
    }
    void hack_member_function_0_192(void*)
    {
        NativeVTableInspector_setResultIndex(192);
    }
    void hack_member_function_0_193(void*)
    {
        NativeVTableInspector_setResultIndex(193);
    }
    void hack_member_function_0_194(void*)
    {
        NativeVTableInspector_setResultIndex(194);
    }
    void hack_member_function_0_195(void*)
    {
        NativeVTableInspector_setResultIndex(195);
    }
    void hack_member_function_0_196(void*)
    {
        NativeVTableInspector_setResultIndex(196);
    }
    void hack_member_function_0_197(void*)
    {
        NativeVTableInspector_setResultIndex(197);
    }
    void hack_member_function_0_198(void*)
    {
        NativeVTableInspector_setResultIndex(198);
    }
    void hack_member_function_0_199(void*)
    {
        NativeVTableInspector_setResultIndex(199);
    }
    void hack_member_function_0_200(void*)
    {
        NativeVTableInspector_setResultIndex(200);
    }
    void hack_member_function_0_201(void*)
    {
        NativeVTableInspector_setResultIndex(201);
    }
    void hack_member_function_0_202(void*)
    {
        NativeVTableInspector_setResultIndex(202);
    }
    void hack_member_function_0_203(void*)
    {
        NativeVTableInspector_setResultIndex(203);
    }
    void hack_member_function_0_204(void*)
    {
        NativeVTableInspector_setResultIndex(204);
    }
    void hack_member_function_0_205(void*)
    {
        NativeVTableInspector_setResultIndex(205);
    }
    void hack_member_function_0_206(void*)
    {
        NativeVTableInspector_setResultIndex(206);
    }
    void hack_member_function_0_207(void*)
    {
        NativeVTableInspector_setResultIndex(207);
    }
    void hack_member_function_0_208(void*)
    {
        NativeVTableInspector_setResultIndex(208);
    }
    void hack_member_function_0_209(void*)
    {
        NativeVTableInspector_setResultIndex(209);
    }
    void hack_member_function_0_210(void*)
    {
        NativeVTableInspector_setResultIndex(210);
    }
    void hack_member_function_0_211(void*)
    {
        NativeVTableInspector_setResultIndex(211);
    }
    void hack_member_function_0_212(void*)
    {
        NativeVTableInspector_setResultIndex(212);
    }
    void hack_member_function_0_213(void*)
    {
        NativeVTableInspector_setResultIndex(213);
    }
    void hack_member_function_0_214(void*)
    {
        NativeVTableInspector_setResultIndex(214);
    }
    void hack_member_function_0_215(void*)
    {
        NativeVTableInspector_setResultIndex(215);
    }
    void hack_member_function_0_216(void*)
    {
        NativeVTableInspector_setResultIndex(216);
    }
    void hack_member_function_0_217(void*)
    {
        NativeVTableInspector_setResultIndex(217);
    }
    void hack_member_function_0_218(void*)
    {
        NativeVTableInspector_setResultIndex(218);
    }
    void hack_member_function_0_219(void*)
    {
        NativeVTableInspector_setResultIndex(219);
    }
    void hack_member_function_0_220(void*)
    {
        NativeVTableInspector_setResultIndex(220);
    }
    void hack_member_function_0_221(void*)
    {
        NativeVTableInspector_setResultIndex(221);
    }
    void hack_member_function_0_222(void*)
    {
        NativeVTableInspector_setResultIndex(222);
    }
    void hack_member_function_0_223(void*)
    {
        NativeVTableInspector_setResultIndex(223);
    }
    void hack_member_function_0_224(void*)
    {
        NativeVTableInspector_setResultIndex(224);
    }
    void hack_member_function_0_225(void*)
    {
        NativeVTableInspector_setResultIndex(225);
    }
    void hack_member_function_0_226(void*)
    {
        NativeVTableInspector_setResultIndex(226);
    }
    void hack_member_function_0_227(void*)
    {
        NativeVTableInspector_setResultIndex(227);
    }
    void hack_member_function_0_228(void*)
    {
        NativeVTableInspector_setResultIndex(228);
    }
    void hack_member_function_0_229(void*)
    {
        NativeVTableInspector_setResultIndex(229);
    }
    void hack_member_function_0_230(void*)
    {
        NativeVTableInspector_setResultIndex(230);
    }
    void hack_member_function_0_231(void*)
    {
        NativeVTableInspector_setResultIndex(231);
    }
    void hack_member_function_0_232(void*)
    {
        NativeVTableInspector_setResultIndex(232);
    }
    void hack_member_function_0_233(void*)
    {
        NativeVTableInspector_setResultIndex(233);
    }
    void hack_member_function_0_234(void*)
    {
        NativeVTableInspector_setResultIndex(234);
    }
    void hack_member_function_0_235(void*)
    {
        NativeVTableInspector_setResultIndex(235);
    }
    void hack_member_function_0_236(void*)
    {
        NativeVTableInspector_setResultIndex(236);
    }
    void hack_member_function_0_237(void*)
    {
        NativeVTableInspector_setResultIndex(237);
    }
    void hack_member_function_0_238(void*)
    {
        NativeVTableInspector_setResultIndex(238);
    }
    void hack_member_function_0_239(void*)
    {
        NativeVTableInspector_setResultIndex(239);
    }
    void hack_member_function_0_240(void*)
    {
        NativeVTableInspector_setResultIndex(240);
    }
    void hack_member_function_0_241(void*)
    {
        NativeVTableInspector_setResultIndex(241);
    }
    void hack_member_function_0_242(void*)
    {
        NativeVTableInspector_setResultIndex(242);
    }
    void hack_member_function_0_243(void*)
    {
        NativeVTableInspector_setResultIndex(243);
    }
    void hack_member_function_0_244(void*)
    {
        NativeVTableInspector_setResultIndex(244);
    }
    void hack_member_function_0_245(void*)
    {
        NativeVTableInspector_setResultIndex(245);
    }
    void hack_member_function_0_246(void*)
    {
        NativeVTableInspector_setResultIndex(246);
    }
    void hack_member_function_0_247(void*)
    {
        NativeVTableInspector_setResultIndex(247);
    }
    void hack_member_function_0_248(void*)
    {
        NativeVTableInspector_setResultIndex(248);
    }
    void hack_member_function_0_249(void*)
    {
        NativeVTableInspector_setResultIndex(249);
    }
    void hack_member_function_1_0(void*)
    {
        NativeVTableInspector_setResultIndex(0 + 250);
    }
    void hack_member_function_1_1(void*)
    {
        NativeVTableInspector_setResultIndex(1 + 250);
    }
    void hack_member_function_1_2(void*)
    {
        NativeVTableInspector_setResultIndex(2 + 250);
    }
    void hack_member_function_1_3(void*)
    {
        NativeVTableInspector_setResultIndex(3 + 250);
    }
    void hack_member_function_1_4(void*)
    {
        NativeVTableInspector_setResultIndex(4 + 250);
    }
    void hack_member_function_1_5(void*)
    {
        NativeVTableInspector_setResultIndex(5 + 250);
    }
    void hack_member_function_1_6(void*)
    {
        NativeVTableInspector_setResultIndex(6 + 250);
    }
    void hack_member_function_1_7(void*)
    {
        NativeVTableInspector_setResultIndex(7 + 250);
    }
    void hack_member_function_1_8(void*)
    {
        NativeVTableInspector_setResultIndex(8 + 250);
    }
    void hack_member_function_1_9(void*)
    {
        NativeVTableInspector_setResultIndex(9 + 250);
    }
    void hack_member_function_1_10(void*)
    {
        NativeVTableInspector_setResultIndex(10 + 250);
    }
    void hack_member_function_1_11(void*)
    {
        NativeVTableInspector_setResultIndex(11 + 250);
    }
    void hack_member_function_1_12(void*)
    {
        NativeVTableInspector_setResultIndex(12 + 250);
    }
    void hack_member_function_1_13(void*)
    {
        NativeVTableInspector_setResultIndex(13 + 250);
    }
    void hack_member_function_1_14(void*)
    {
        NativeVTableInspector_setResultIndex(14 + 250);
    }
    void hack_member_function_1_15(void*)
    {
        NativeVTableInspector_setResultIndex(15 + 250);
    }
    void hack_member_function_1_16(void*)
    {
        NativeVTableInspector_setResultIndex(16 + 250);
    }
    void hack_member_function_1_17(void*)
    {
        NativeVTableInspector_setResultIndex(17 + 250);
    }
    void hack_member_function_1_18(void*)
    {
        NativeVTableInspector_setResultIndex(18 + 250);
    }
    void hack_member_function_1_19(void*)
    {
        NativeVTableInspector_setResultIndex(19 + 250);
    }
    void hack_member_function_1_20(void*)
    {
        NativeVTableInspector_setResultIndex(20 + 250);
    }
    void hack_member_function_1_21(void*)
    {
        NativeVTableInspector_setResultIndex(21 + 250);
    }
    void hack_member_function_1_22(void*)
    {
        NativeVTableInspector_setResultIndex(22 + 250);
    }
    void hack_member_function_1_23(void*)
    {
        NativeVTableInspector_setResultIndex(23 + 250);
    }
    void hack_member_function_1_24(void*)
    {
        NativeVTableInspector_setResultIndex(24 + 250);
    }
    void hack_member_function_1_25(void*)
    {
        NativeVTableInspector_setResultIndex(25 + 250);
    }
    void hack_member_function_1_26(void*)
    {
        NativeVTableInspector_setResultIndex(26 + 250);
    }
    void hack_member_function_1_27(void*)
    {
        NativeVTableInspector_setResultIndex(27 + 250);
    }
    void hack_member_function_1_28(void*)
    {
        NativeVTableInspector_setResultIndex(28 + 250);
    }
    void hack_member_function_1_29(void*)
    {
        NativeVTableInspector_setResultIndex(29 + 250);
    }
    void hack_member_function_1_30(void*)
    {
        NativeVTableInspector_setResultIndex(30 + 250);
    }
    void hack_member_function_1_31(void*)
    {
        NativeVTableInspector_setResultIndex(31 + 250);
    }
    void hack_member_function_1_32(void*)
    {
        NativeVTableInspector_setResultIndex(32 + 250);
    }
    void hack_member_function_1_33(void*)
    {
        NativeVTableInspector_setResultIndex(33 + 250);
    }
    void hack_member_function_1_34(void*)
    {
        NativeVTableInspector_setResultIndex(34 + 250);
    }
    void hack_member_function_1_35(void*)
    {
        NativeVTableInspector_setResultIndex(35 + 250);
    }
    void hack_member_function_1_36(void*)
    {
        NativeVTableInspector_setResultIndex(36 + 250);
    }
    void hack_member_function_1_37(void*)
    {
        NativeVTableInspector_setResultIndex(37 + 250);
    }
    void hack_member_function_1_38(void*)
    {
        NativeVTableInspector_setResultIndex(38 + 250);
    }
    void hack_member_function_1_39(void*)
    {
        NativeVTableInspector_setResultIndex(39 + 250);
    }
    void hack_member_function_1_40(void*)
    {
        NativeVTableInspector_setResultIndex(40 + 250);
    }
    void hack_member_function_1_41(void*)
    {
        NativeVTableInspector_setResultIndex(41 + 250);
    }
    void hack_member_function_1_42(void*)
    {
        NativeVTableInspector_setResultIndex(42 + 250);
    }
    void hack_member_function_1_43(void*)
    {
        NativeVTableInspector_setResultIndex(43 + 250);
    }
    void hack_member_function_1_44(void*)
    {
        NativeVTableInspector_setResultIndex(44 + 250);
    }
    void hack_member_function_1_45(void*)
    {
        NativeVTableInspector_setResultIndex(45 + 250);
    }
    void hack_member_function_1_46(void*)
    {
        NativeVTableInspector_setResultIndex(46 + 250);
    }
    void hack_member_function_1_47(void*)
    {
        NativeVTableInspector_setResultIndex(47 + 250);
    }
    void hack_member_function_1_48(void*)
    {
        NativeVTableInspector_setResultIndex(48 + 250);
    }
    void hack_member_function_1_49(void*)
    {
        NativeVTableInspector_setResultIndex(49 + 250);
    }
    void hack_member_function_1_50(void*)
    {
        NativeVTableInspector_setResultIndex(50 + 250);
    }
    void hack_member_function_1_51(void*)
    {
        NativeVTableInspector_setResultIndex(51 + 250);
    }
    void hack_member_function_1_52(void*)
    {
        NativeVTableInspector_setResultIndex(52 + 250);
    }
    void hack_member_function_1_53(void*)
    {
        NativeVTableInspector_setResultIndex(53 + 250);
    }
    void hack_member_function_1_54(void*)
    {
        NativeVTableInspector_setResultIndex(54 + 250);
    }
    void hack_member_function_1_55(void*)
    {
        NativeVTableInspector_setResultIndex(55 + 250);
    }
    void hack_member_function_1_56(void*)
    {
        NativeVTableInspector_setResultIndex(56 + 250);
    }
    void hack_member_function_1_57(void*)
    {
        NativeVTableInspector_setResultIndex(57 + 250);
    }
    void hack_member_function_1_58(void*)
    {
        NativeVTableInspector_setResultIndex(58 + 250);
    }
    void hack_member_function_1_59(void*)
    {
        NativeVTableInspector_setResultIndex(59 + 250);
    }
    void hack_member_function_1_60(void*)
    {
        NativeVTableInspector_setResultIndex(60 + 250);
    }
    void hack_member_function_1_61(void*)
    {
        NativeVTableInspector_setResultIndex(61 + 250);
    }
    void hack_member_function_1_62(void*)
    {
        NativeVTableInspector_setResultIndex(62 + 250);
    }
    void hack_member_function_1_63(void*)
    {
        NativeVTableInspector_setResultIndex(63 + 250);
    }
    void hack_member_function_1_64(void*)
    {
        NativeVTableInspector_setResultIndex(64 + 250);
    }
    void hack_member_function_1_65(void*)
    {
        NativeVTableInspector_setResultIndex(65 + 250);
    }
    void hack_member_function_1_66(void*)
    {
        NativeVTableInspector_setResultIndex(66 + 250);
    }
    void hack_member_function_1_67(void*)
    {
        NativeVTableInspector_setResultIndex(67 + 250);
    }
    void hack_member_function_1_68(void*)
    {
        NativeVTableInspector_setResultIndex(68 + 250);
    }
    void hack_member_function_1_69(void*)
    {
        NativeVTableInspector_setResultIndex(69 + 250);
    }
    void hack_member_function_1_70(void*)
    {
        NativeVTableInspector_setResultIndex(70 + 250);
    }
    void hack_member_function_1_71(void*)
    {
        NativeVTableInspector_setResultIndex(71 + 250);
    }
    void hack_member_function_1_72(void*)
    {
        NativeVTableInspector_setResultIndex(72 + 250);
    }
    void hack_member_function_1_73(void*)
    {
        NativeVTableInspector_setResultIndex(73 + 250);
    }
    void hack_member_function_1_74(void*)
    {
        NativeVTableInspector_setResultIndex(74 + 250);
    }
    void hack_member_function_1_75(void*)
    {
        NativeVTableInspector_setResultIndex(75 + 250);
    }
    void hack_member_function_1_76(void*)
    {
        NativeVTableInspector_setResultIndex(76 + 250);
    }
    void hack_member_function_1_77(void*)
    {
        NativeVTableInspector_setResultIndex(77 + 250);
    }
    void hack_member_function_1_78(void*)
    {
        NativeVTableInspector_setResultIndex(78 + 250);
    }
    void hack_member_function_1_79(void*)
    {
        NativeVTableInspector_setResultIndex(79 + 250);
    }
    void hack_member_function_1_80(void*)
    {
        NativeVTableInspector_setResultIndex(80 + 250);
    }
    void hack_member_function_1_81(void*)
    {
        NativeVTableInspector_setResultIndex(81 + 250);
    }
    void hack_member_function_1_82(void*)
    {
        NativeVTableInspector_setResultIndex(82 + 250);
    }
    void hack_member_function_1_83(void*)
    {
        NativeVTableInspector_setResultIndex(83 + 250);
    }
    void hack_member_function_1_84(void*)
    {
        NativeVTableInspector_setResultIndex(84 + 250);
    }
    void hack_member_function_1_85(void*)
    {
        NativeVTableInspector_setResultIndex(85 + 250);
    }
    void hack_member_function_1_86(void*)
    {
        NativeVTableInspector_setResultIndex(86 + 250);
    }
    void hack_member_function_1_87(void*)
    {
        NativeVTableInspector_setResultIndex(87 + 250);
    }
    void hack_member_function_1_88(void*)
    {
        NativeVTableInspector_setResultIndex(88 + 250);
    }
    void hack_member_function_1_89(void*)
    {
        NativeVTableInspector_setResultIndex(89 + 250);
    }
    void hack_member_function_1_90(void*)
    {
        NativeVTableInspector_setResultIndex(90 + 250);
    }
    void hack_member_function_1_91(void*)
    {
        NativeVTableInspector_setResultIndex(91 + 250);
    }
    void hack_member_function_1_92(void*)
    {
        NativeVTableInspector_setResultIndex(92 + 250);
    }
    void hack_member_function_1_93(void*)
    {
        NativeVTableInspector_setResultIndex(93 + 250);
    }
    void hack_member_function_1_94(void*)
    {
        NativeVTableInspector_setResultIndex(94 + 250);
    }
    void hack_member_function_1_95(void*)
    {
        NativeVTableInspector_setResultIndex(95 + 250);
    }
    void hack_member_function_1_96(void*)
    {
        NativeVTableInspector_setResultIndex(96 + 250);
    }
    void hack_member_function_1_97(void*)
    {
        NativeVTableInspector_setResultIndex(97 + 250);
    }
    void hack_member_function_1_98(void*)
    {
        NativeVTableInspector_setResultIndex(98 + 250);
    }
    void hack_member_function_1_99(void*)
    {
        NativeVTableInspector_setResultIndex(99 + 250);
    }
    void hack_member_function_1_100(void*)
    {
        NativeVTableInspector_setResultIndex(100 + 250);
    }
    void hack_member_function_1_101(void*)
    {
        NativeVTableInspector_setResultIndex(101 + 250);
    }
    void hack_member_function_1_102(void*)
    {
        NativeVTableInspector_setResultIndex(102 + 250);
    }
    void hack_member_function_1_103(void*)
    {
        NativeVTableInspector_setResultIndex(103 + 250);
    }
    void hack_member_function_1_104(void*)
    {
        NativeVTableInspector_setResultIndex(104 + 250);
    }
    void hack_member_function_1_105(void*)
    {
        NativeVTableInspector_setResultIndex(105 + 250);
    }
    void hack_member_function_1_106(void*)
    {
        NativeVTableInspector_setResultIndex(106 + 250);
    }
    void hack_member_function_1_107(void*)
    {
        NativeVTableInspector_setResultIndex(107 + 250);
    }
    void hack_member_function_1_108(void*)
    {
        NativeVTableInspector_setResultIndex(108 + 250);
    }
    void hack_member_function_1_109(void*)
    {
        NativeVTableInspector_setResultIndex(109 + 250);
    }
    void hack_member_function_1_110(void*)
    {
        NativeVTableInspector_setResultIndex(110 + 250);
    }
    void hack_member_function_1_111(void*)
    {
        NativeVTableInspector_setResultIndex(111 + 250);
    }
    void hack_member_function_1_112(void*)
    {
        NativeVTableInspector_setResultIndex(112 + 250);
    }
    void hack_member_function_1_113(void*)
    {
        NativeVTableInspector_setResultIndex(113 + 250);
    }
    void hack_member_function_1_114(void*)
    {
        NativeVTableInspector_setResultIndex(114 + 250);
    }
    void hack_member_function_1_115(void*)
    {
        NativeVTableInspector_setResultIndex(115 + 250);
    }
    void hack_member_function_1_116(void*)
    {
        NativeVTableInspector_setResultIndex(116 + 250);
    }
    void hack_member_function_1_117(void*)
    {
        NativeVTableInspector_setResultIndex(117 + 250);
    }
    void hack_member_function_1_118(void*)
    {
        NativeVTableInspector_setResultIndex(118 + 250);
    }
    void hack_member_function_1_119(void*)
    {
        NativeVTableInspector_setResultIndex(119 + 250);
    }
    void hack_member_function_1_120(void*)
    {
        NativeVTableInspector_setResultIndex(120 + 250);
    }
    void hack_member_function_1_121(void*)
    {
        NativeVTableInspector_setResultIndex(121 + 250);
    }
    void hack_member_function_1_122(void*)
    {
        NativeVTableInspector_setResultIndex(122 + 250);
    }
    void hack_member_function_1_123(void*)
    {
        NativeVTableInspector_setResultIndex(123 + 250);
    }
    void hack_member_function_1_124(void*)
    {
        NativeVTableInspector_setResultIndex(124 + 250);
    }
    void hack_member_function_1_125(void*)
    {
        NativeVTableInspector_setResultIndex(125 + 250);
    }
    void hack_member_function_1_126(void*)
    {
        NativeVTableInspector_setResultIndex(126 + 250);
    }
    void hack_member_function_1_127(void*)
    {
        NativeVTableInspector_setResultIndex(127 + 250);
    }
    void hack_member_function_1_128(void*)
    {
        NativeVTableInspector_setResultIndex(128 + 250);
    }
    void hack_member_function_1_129(void*)
    {
        NativeVTableInspector_setResultIndex(129 + 250);
    }
    void hack_member_function_1_130(void*)
    {
        NativeVTableInspector_setResultIndex(130 + 250);
    }
    void hack_member_function_1_131(void*)
    {
        NativeVTableInspector_setResultIndex(131 + 250);
    }
    void hack_member_function_1_132(void*)
    {
        NativeVTableInspector_setResultIndex(132 + 250);
    }
    void hack_member_function_1_133(void*)
    {
        NativeVTableInspector_setResultIndex(133 + 250);
    }
    void hack_member_function_1_134(void*)
    {
        NativeVTableInspector_setResultIndex(134 + 250);
    }
    void hack_member_function_1_135(void*)
    {
        NativeVTableInspector_setResultIndex(135 + 250);
    }
    void hack_member_function_1_136(void*)
    {
        NativeVTableInspector_setResultIndex(136 + 250);
    }
    void hack_member_function_1_137(void*)
    {
        NativeVTableInspector_setResultIndex(137 + 250);
    }
    void hack_member_function_1_138(void*)
    {
        NativeVTableInspector_setResultIndex(138 + 250);
    }
    void hack_member_function_1_139(void*)
    {
        NativeVTableInspector_setResultIndex(139 + 250);
    }
    void hack_member_function_1_140(void*)
    {
        NativeVTableInspector_setResultIndex(140 + 250);
    }
    void hack_member_function_1_141(void*)
    {
        NativeVTableInspector_setResultIndex(141 + 250);
    }
    void hack_member_function_1_142(void*)
    {
        NativeVTableInspector_setResultIndex(142 + 250);
    }
    void hack_member_function_1_143(void*)
    {
        NativeVTableInspector_setResultIndex(143 + 250);
    }
    void hack_member_function_1_144(void*)
    {
        NativeVTableInspector_setResultIndex(144 + 250);
    }
    void hack_member_function_1_145(void*)
    {
        NativeVTableInspector_setResultIndex(145 + 250);
    }
    void hack_member_function_1_146(void*)
    {
        NativeVTableInspector_setResultIndex(146 + 250);
    }
    void hack_member_function_1_147(void*)
    {
        NativeVTableInspector_setResultIndex(147 + 250);
    }
    void hack_member_function_1_148(void*)
    {
        NativeVTableInspector_setResultIndex(148 + 250);
    }
    void hack_member_function_1_149(void*)
    {
        NativeVTableInspector_setResultIndex(149 + 250);
    }
    void hack_member_function_1_150(void*)
    {
        NativeVTableInspector_setResultIndex(150 + 250);
    }
    void hack_member_function_1_151(void*)
    {
        NativeVTableInspector_setResultIndex(151 + 250);
    }
    void hack_member_function_1_152(void*)
    {
        NativeVTableInspector_setResultIndex(152 + 250);
    }
    void hack_member_function_1_153(void*)
    {
        NativeVTableInspector_setResultIndex(153 + 250);
    }
    void hack_member_function_1_154(void*)
    {
        NativeVTableInspector_setResultIndex(154 + 250);
    }
    void hack_member_function_1_155(void*)
    {
        NativeVTableInspector_setResultIndex(155 + 250);
    }
    void hack_member_function_1_156(void*)
    {
        NativeVTableInspector_setResultIndex(156 + 250);
    }
    void hack_member_function_1_157(void*)
    {
        NativeVTableInspector_setResultIndex(157 + 250);
    }
    void hack_member_function_1_158(void*)
    {
        NativeVTableInspector_setResultIndex(158 + 250);
    }
    void hack_member_function_1_159(void*)
    {
        NativeVTableInspector_setResultIndex(159 + 250);
    }
    void hack_member_function_1_160(void*)
    {
        NativeVTableInspector_setResultIndex(160 + 250);
    }
    void hack_member_function_1_161(void*)
    {
        NativeVTableInspector_setResultIndex(161 + 250);
    }
    void hack_member_function_1_162(void*)
    {
        NativeVTableInspector_setResultIndex(162 + 250);
    }
    void hack_member_function_1_163(void*)
    {
        NativeVTableInspector_setResultIndex(163 + 250);
    }
    void hack_member_function_1_164(void*)
    {
        NativeVTableInspector_setResultIndex(164 + 250);
    }
    void hack_member_function_1_165(void*)
    {
        NativeVTableInspector_setResultIndex(165 + 250);
    }
    void hack_member_function_1_166(void*)
    {
        NativeVTableInspector_setResultIndex(166 + 250);
    }
    void hack_member_function_1_167(void*)
    {
        NativeVTableInspector_setResultIndex(167 + 250);
    }
    void hack_member_function_1_168(void*)
    {
        NativeVTableInspector_setResultIndex(168 + 250);
    }
    void hack_member_function_1_169(void*)
    {
        NativeVTableInspector_setResultIndex(169 + 250);
    }
    void hack_member_function_1_170(void*)
    {
        NativeVTableInspector_setResultIndex(170 + 250);
    }
    void hack_member_function_1_171(void*)
    {
        NativeVTableInspector_setResultIndex(171 + 250);
    }
    void hack_member_function_1_172(void*)
    {
        NativeVTableInspector_setResultIndex(172 + 250);
    }
    void hack_member_function_1_173(void*)
    {
        NativeVTableInspector_setResultIndex(173 + 250);
    }
    void hack_member_function_1_174(void*)
    {
        NativeVTableInspector_setResultIndex(174 + 250);
    }
    void hack_member_function_1_175(void*)
    {
        NativeVTableInspector_setResultIndex(175 + 250);
    }
    void hack_member_function_1_176(void*)
    {
        NativeVTableInspector_setResultIndex(176 + 250);
    }
    void hack_member_function_1_177(void*)
    {
        NativeVTableInspector_setResultIndex(177 + 250);
    }
    void hack_member_function_1_178(void*)
    {
        NativeVTableInspector_setResultIndex(178 + 250);
    }
    void hack_member_function_1_179(void*)
    {
        NativeVTableInspector_setResultIndex(179 + 250);
    }
    void hack_member_function_1_180(void*)
    {
        NativeVTableInspector_setResultIndex(180 + 250);
    }
    void hack_member_function_1_181(void*)
    {
        NativeVTableInspector_setResultIndex(181 + 250);
    }
    void hack_member_function_1_182(void*)
    {
        NativeVTableInspector_setResultIndex(182 + 250);
    }
    void hack_member_function_1_183(void*)
    {
        NativeVTableInspector_setResultIndex(183 + 250);
    }
    void hack_member_function_1_184(void*)
    {
        NativeVTableInspector_setResultIndex(184 + 250);
    }
    void hack_member_function_1_185(void*)
    {
        NativeVTableInspector_setResultIndex(185 + 250);
    }
    void hack_member_function_1_186(void*)
    {
        NativeVTableInspector_setResultIndex(186 + 250);
    }
    void hack_member_function_1_187(void*)
    {
        NativeVTableInspector_setResultIndex(187 + 250);
    }
    void hack_member_function_1_188(void*)
    {
        NativeVTableInspector_setResultIndex(188 + 250);
    }
    void hack_member_function_1_189(void*)
    {
        NativeVTableInspector_setResultIndex(189 + 250);
    }
    void hack_member_function_1_190(void*)
    {
        NativeVTableInspector_setResultIndex(190 + 250);
    }
    void hack_member_function_1_191(void*)
    {
        NativeVTableInspector_setResultIndex(191 + 250);
    }
    void hack_member_function_1_192(void*)
    {
        NativeVTableInspector_setResultIndex(192 + 250);
    }
    void hack_member_function_1_193(void*)
    {
        NativeVTableInspector_setResultIndex(193 + 250);
    }
    void hack_member_function_1_194(void*)
    {
        NativeVTableInspector_setResultIndex(194 + 250);
    }
    void hack_member_function_1_195(void*)
    {
        NativeVTableInspector_setResultIndex(195 + 250);
    }
    void hack_member_function_1_196(void*)
    {
        NativeVTableInspector_setResultIndex(196 + 250);
    }
    void hack_member_function_1_197(void*)
    {
        NativeVTableInspector_setResultIndex(197 + 250);
    }
    void hack_member_function_1_198(void*)
    {
        NativeVTableInspector_setResultIndex(198 + 250);
    }
    void hack_member_function_1_199(void*)
    {
        NativeVTableInspector_setResultIndex(199 + 250);
    }
    void hack_member_function_1_200(void*)
    {
        NativeVTableInspector_setResultIndex(200 + 250);
    }
    void hack_member_function_1_201(void*)
    {
        NativeVTableInspector_setResultIndex(201 + 250);
    }
    void hack_member_function_1_202(void*)
    {
        NativeVTableInspector_setResultIndex(202 + 250);
    }
    void hack_member_function_1_203(void*)
    {
        NativeVTableInspector_setResultIndex(203 + 250);
    }
    void hack_member_function_1_204(void*)
    {
        NativeVTableInspector_setResultIndex(204 + 250);
    }
    void hack_member_function_1_205(void*)
    {
        NativeVTableInspector_setResultIndex(205 + 250);
    }
    void hack_member_function_1_206(void*)
    {
        NativeVTableInspector_setResultIndex(206 + 250);
    }
    void hack_member_function_1_207(void*)
    {
        NativeVTableInspector_setResultIndex(207 + 250);
    }
    void hack_member_function_1_208(void*)
    {
        NativeVTableInspector_setResultIndex(208 + 250);
    }
    void hack_member_function_1_209(void*)
    {
        NativeVTableInspector_setResultIndex(209 + 250);
    }
    void hack_member_function_1_210(void*)
    {
        NativeVTableInspector_setResultIndex(210 + 250);
    }
    void hack_member_function_1_211(void*)
    {
        NativeVTableInspector_setResultIndex(211 + 250);
    }
    void hack_member_function_1_212(void*)
    {
        NativeVTableInspector_setResultIndex(212 + 250);
    }
    void hack_member_function_1_213(void*)
    {
        NativeVTableInspector_setResultIndex(213 + 250);
    }
    void hack_member_function_1_214(void*)
    {
        NativeVTableInspector_setResultIndex(214 + 250);
    }
    void hack_member_function_1_215(void*)
    {
        NativeVTableInspector_setResultIndex(215 + 250);
    }
    void hack_member_function_1_216(void*)
    {
        NativeVTableInspector_setResultIndex(216 + 250);
    }
    void hack_member_function_1_217(void*)
    {
        NativeVTableInspector_setResultIndex(217 + 250);
    }
    void hack_member_function_1_218(void*)
    {
        NativeVTableInspector_setResultIndex(218 + 250);
    }
    void hack_member_function_1_219(void*)
    {
        NativeVTableInspector_setResultIndex(219 + 250);
    }
    void hack_member_function_1_220(void*)
    {
        NativeVTableInspector_setResultIndex(220 + 250);
    }
    void hack_member_function_1_221(void*)
    {
        NativeVTableInspector_setResultIndex(221 + 250);
    }
    void hack_member_function_1_222(void*)
    {
        NativeVTableInspector_setResultIndex(222 + 250);
    }
    void hack_member_function_1_223(void*)
    {
        NativeVTableInspector_setResultIndex(223 + 250);
    }
    void hack_member_function_1_224(void*)
    {
        NativeVTableInspector_setResultIndex(224 + 250);
    }
    void hack_member_function_1_225(void*)
    {
        NativeVTableInspector_setResultIndex(225 + 250);
    }
    void hack_member_function_1_226(void*)
    {
        NativeVTableInspector_setResultIndex(226 + 250);
    }
    void hack_member_function_1_227(void*)
    {
        NativeVTableInspector_setResultIndex(227 + 250);
    }
    void hack_member_function_1_228(void*)
    {
        NativeVTableInspector_setResultIndex(228 + 250);
    }
    void hack_member_function_1_229(void*)
    {
        NativeVTableInspector_setResultIndex(229 + 250);
    }
    void hack_member_function_1_230(void*)
    {
        NativeVTableInspector_setResultIndex(230 + 250);
    }
    void hack_member_function_1_231(void*)
    {
        NativeVTableInspector_setResultIndex(231 + 250);
    }
    void hack_member_function_1_232(void*)
    {
        NativeVTableInspector_setResultIndex(232 + 250);
    }
    void hack_member_function_1_233(void*)
    {
        NativeVTableInspector_setResultIndex(233 + 250);
    }
    void hack_member_function_1_234(void*)
    {
        NativeVTableInspector_setResultIndex(234 + 250);
    }
    void hack_member_function_1_235(void*)
    {
        NativeVTableInspector_setResultIndex(235 + 250);
    }
    void hack_member_function_1_236(void*)
    {
        NativeVTableInspector_setResultIndex(236 + 250);
    }
    void hack_member_function_1_237(void*)
    {
        NativeVTableInspector_setResultIndex(237 + 250);
    }
    void hack_member_function_1_238(void*)
    {
        NativeVTableInspector_setResultIndex(238 + 250);
    }
    void hack_member_function_1_239(void*)
    {
        NativeVTableInspector_setResultIndex(239 + 250);
    }
    void hack_member_function_1_240(void*)
    {
        NativeVTableInspector_setResultIndex(240 + 250);
    }
    void hack_member_function_1_241(void*)
    {
        NativeVTableInspector_setResultIndex(241 + 250);
    }
    void hack_member_function_1_242(void*)
    {
        NativeVTableInspector_setResultIndex(242 + 250);
    }
    void hack_member_function_1_243(void*)
    {
        NativeVTableInspector_setResultIndex(243 + 250);
    }
    void hack_member_function_1_244(void*)
    {
        NativeVTableInspector_setResultIndex(244 + 250);
    }
    void hack_member_function_1_245(void*)
    {
        NativeVTableInspector_setResultIndex(245 + 250);
    }
    void hack_member_function_1_246(void*)
    {
        NativeVTableInspector_setResultIndex(246 + 250);
    }
    void hack_member_function_1_247(void*)
    {
        NativeVTableInspector_setResultIndex(247 + 250);
    }
    void hack_member_function_1_248(void*)
    {
        NativeVTableInspector_setResultIndex(248 + 250);
    }
    void hack_member_function_1_249(void*)
    {
        NativeVTableInspector_setResultIndex(249 + 250);
    }
};

VTableInspectorGeneric::VTableInspectorGeneric()
{
    static void* vptr_imspostor[512 + 1] = {
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_0),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_1),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_2),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_3),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_4),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_5),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_6),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_7),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_8),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_9),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_10),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_11),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_12),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_13),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_14),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_15),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_16),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_17),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_18),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_19),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_20),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_21),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_22),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_23),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_24),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_25),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_26),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_27),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_28),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_29),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_30),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_31),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_32),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_33),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_34),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_35),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_36),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_37),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_38),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_39),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_40),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_41),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_42),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_43),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_44),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_45),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_46),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_47),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_48),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_49),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_50),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_51),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_52),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_53),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_54),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_55),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_56),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_57),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_58),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_59),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_60),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_61),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_62),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_63),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_64),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_65),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_66),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_67),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_68),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_69),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_70),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_71),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_72),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_73),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_74),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_75),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_76),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_77),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_78),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_79),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_80),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_81),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_82),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_83),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_84),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_85),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_86),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_87),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_88),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_89),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_90),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_91),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_92),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_93),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_94),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_95),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_96),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_97),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_98),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_99),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_100),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_101),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_102),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_103),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_104),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_105),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_106),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_107),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_108),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_109),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_110),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_111),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_112),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_113),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_114),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_115),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_116),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_117),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_118),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_119),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_120),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_121),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_122),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_123),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_124),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_125),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_126),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_127),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_128),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_129),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_130),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_131),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_132),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_133),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_134),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_135),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_136),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_137),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_138),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_139),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_140),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_141),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_142),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_143),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_144),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_145),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_146),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_147),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_148),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_149),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_150),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_151),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_152),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_153),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_154),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_155),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_156),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_157),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_158),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_159),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_160),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_161),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_162),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_163),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_164),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_165),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_166),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_167),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_168),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_169),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_170),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_171),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_172),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_173),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_174),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_175),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_176),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_177),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_178),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_179),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_180),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_181),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_182),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_183),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_184),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_185),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_186),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_187),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_188),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_189),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_190),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_191),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_192),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_193),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_194),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_195),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_196),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_197),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_198),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_199),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_200),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_201),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_202),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_203),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_204),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_205),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_206),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_207),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_208),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_209),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_210),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_211),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_212),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_213),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_214),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_215),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_216),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_217),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_218),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_219),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_220),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_221),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_222),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_223),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_224),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_225),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_226),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_227),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_228),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_229),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_230),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_231),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_232),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_233),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_234),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_235),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_236),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_237),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_238),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_239),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_240),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_241),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_242),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_243),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_244),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_245),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_246),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_247),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_248),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_0_249),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_0),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_1),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_2),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_3),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_4),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_5),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_6),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_7),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_8),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_9),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_10),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_11),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_12),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_13),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_14),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_15),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_16),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_17),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_18),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_19),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_20),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_21),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_22),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_23),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_24),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_25),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_26),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_27),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_28),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_29),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_30),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_31),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_32),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_33),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_34),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_35),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_36),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_37),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_38),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_39),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_40),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_41),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_42),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_43),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_44),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_45),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_46),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_47),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_48),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_49),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_50),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_51),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_52),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_53),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_54),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_55),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_56),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_57),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_58),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_59),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_60),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_61),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_62),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_63),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_64),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_65),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_66),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_67),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_68),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_69),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_70),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_71),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_72),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_73),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_74),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_75),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_76),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_77),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_78),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_79),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_80),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_81),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_82),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_83),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_84),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_85),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_86),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_87),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_88),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_89),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_90),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_91),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_92),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_93),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_94),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_95),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_96),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_97),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_98),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_99),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_100),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_101),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_102),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_103),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_104),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_105),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_106),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_107),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_108),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_109),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_110),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_111),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_112),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_113),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_114),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_115),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_116),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_117),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_118),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_119),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_120),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_121),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_122),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_123),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_124),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_125),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_126),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_127),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_128),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_129),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_130),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_131),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_132),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_133),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_134),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_135),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_136),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_137),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_138),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_139),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_140),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_141),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_142),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_143),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_144),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_145),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_146),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_147),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_148),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_149),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_150),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_151),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_152),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_153),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_154),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_155),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_156),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_157),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_158),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_159),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_160),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_161),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_162),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_163),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_164),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_165),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_166),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_167),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_168),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_169),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_170),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_171),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_172),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_173),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_174),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_175),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_176),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_177),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_178),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_179),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_180),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_181),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_182),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_183),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_184),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_185),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_186),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_187),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_188),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_189),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_190),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_191),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_192),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_193),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_194),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_195),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_196),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_197),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_198),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_199),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_200),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_201),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_202),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_203),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_204),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_205),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_206),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_207),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_208),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_209),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_210),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_211),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_212),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_213),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_214),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_215),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_216),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_217),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_218),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_219),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_220),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_221),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_222),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_223),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_224),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_225),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_226),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_227),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_228),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_229),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_230),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_231),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_232),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_233),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_234),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_235),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_236),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_237),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_238),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_239),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_240),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_241),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_242),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_243),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_244),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_245),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_246),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_247),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_248),
    extract_mfp_closure(&VTableInspectorGeneric_P::hack_member_function_1_249),
    __null};
    m_vptr_impostor = &vptr_imspostor;
}

} // namespace lang
} // namespace phantom
