#include "G2/ThemeManager.hpp"

#include <SDK/SDK/BP_SurvivalGameInstance_classes.hpp>
#include <SDK/SDK/Engine_classes.hpp>
#include <SDK/SDK/GlobalColorTheme_Base_classes.hpp>

SINGLETON_IMPL(ThemeManager);

auto ThemeManager::init_singleton() -> void { refresh(); }

auto ThemeManager::refresh() -> void {
  m_theme = {};

  auto *world = SDK::UWorld::GetWorld();
  if (!world) {
    spdlog::warn("World was null");
    return;
  }

  auto *game_instance_base = world->OwningGameInstance;
  if (!game_instance_base ||
      !game_instance_base->IsA(
          SDK::UBP_SurvivalGameInstance_C::StaticClass())) {
    spdlog::warn("World game instance was invalid");
    return;
  }

  auto *game_instance =
      static_cast<SDK::UBP_SurvivalGameInstance_C *>(game_instance_base);

  SDK::UGlobalColorTheme_Base_C *theme = nullptr;
  game_instance->GetCurrentUIThemeInstance(&theme);
  if (!theme) {
    spdlog::warn("Could not get theme");
    return;
  }

  auto &list = theme->GlobalColorList;

  m_theme.SCABosBase = *reinterpret_cast<std::array<float, 4> *>(
      &list.SCABosBase_87_99E08A38437A85A7846EA1842FA8EE24);
  m_theme.SCABosAccentOne = *reinterpret_cast<std::array<float, 4> *>(
      &list.SCABosAccentOne_88_3CCA0512412C38E4F32D2E99B88F5217);
  m_theme.SCABOSAccentTwo = *reinterpret_cast<std::array<float, 4> *>(
      &list.SCABOSAccentTwo_89_8B01C63240876D377E4C0EB9CB63A33B);
  m_theme.SCABOSAccentThree = *reinterpret_cast<std::array<float, 4> *>(
      &list.SCABOSAccentThree_90_312107B946678C840AD176AB61F13DC1);
  m_theme.SCABOSWarningOne = *reinterpret_cast<std::array<float, 4> *>(
      &list.SCABOSWarningOne_91_FAEA5A634AD3888D68B5B1AD6CADC9EE);
  m_theme.ResourceButton = *reinterpret_cast<std::array<float, 4> *>(
      &list.ResourceButton_92_303246F340BE6C2F2C874AA22A67CC5C);
  m_theme.ResourceButtonSelectedEnabled = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.ResourceButtonSelectedEnabled_93_8666835640E08835B7FCAFA477EF6567);
  m_theme.ResourceAccent = *reinterpret_cast<std::array<float, 4> *>(
      &list.ResourceAccent_94_3AB19D2145A42EF31F5F59929D553A90);
  m_theme.CraftableButtonEnabled = *reinterpret_cast<std::array<float, 4> *>(
      &list.CraftableButtonEnabled_95_7AC954CF420F84265E49AB8F9CE1909B);
  m_theme.CraftableButtonSelectedEnabled = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.CraftableButtonSelectedEnabled_96_46B24F634EAD328ED6D95DA3930A884A);
  m_theme.CraftableButtonSelectedDisabled = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.CraftableButtonSelectedDisabled_97_604AE52244741BAEB85ABCAF19E52075);
  m_theme.CraftableAccentEnabled = *reinterpret_cast<std::array<float, 4> *>(
      &list.CraftableAccentEnabled_98_37BB86524BB05B66BC3F558B512EE70C);
  m_theme.CraftableButtonDisabled = *reinterpret_cast<std::array<float, 4> *>(
      &list.CraftableButtonDisabled_99_BE4F41F74BFBA2DA093B448D1DD27F1C);
  m_theme.CraftableAccentDisabled = *reinterpret_cast<std::array<float, 4> *>(
      &list.CraftableAccentDisabled_100_D04BC9F0498E73A9E853A78F26911919);
  m_theme.BuildingButtonEnabled = *reinterpret_cast<std::array<float, 4> *>(
      &list.BuildingButtonEnabled_101_A89306294E511B8C96AC2FA296B8A8F0);
  m_theme.BuildingButtonSelectedEnabled = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.BuildingButtonSelectedEnabled_102_C10580C744E43DE7A01EF6A521668FA6);
  m_theme.BuildingAccentEnabled = *reinterpret_cast<std::array<float, 4> *>(
      &list.BuildingAccentEnabled_103_5C1AA36A444B932F758BDA9E6F700050);
  m_theme.BuildingButtonDisabled = *reinterpret_cast<std::array<float, 4> *>(
      &list.BuildingButtonDisabled_104_77568F4B4CA96737E52D00A9FB5C6455);
  m_theme.BuildingAccentDisabled = *reinterpret_cast<std::array<float, 4> *>(
      &list.BuildingAccentDisabled_105_AB1B730C461DFF976CAD2BBCB1653870);
  m_theme.BottomButton = *reinterpret_cast<std::array<float, 4> *>(
      &list.BottomButton_106_CF0A32B64995DFE0A3D08FB6473C3756);
  m_theme.BottomButtonDisabled = *reinterpret_cast<std::array<float, 4> *>(
      &list.BottomButtonDisabled_107_A4D0262B487D3362846F84BFC72668F2);
  m_theme.Selecting = *reinterpret_cast<std::array<float, 4> *>(
      &list.Selecting_109_189F43FA40A845CA1C507AB7DB16D6AD);
  m_theme.Selected = *reinterpret_cast<std::array<float, 4> *>(
      &list.Selected_110_34D3AEC3480635500157CFA7C48F02F7);
  m_theme.Food = *reinterpret_cast<std::array<float, 4> *>(
      &list.Food_111_594315EC469A45D4977DA1BD1056B4AF);
  m_theme.Health = *reinterpret_cast<std::array<float, 4> *>(
      &list.Health_86_4F81EF2B4B10338E222D838F1AAF0572);
  m_theme.Water = *reinterpret_cast<std::array<float, 4> *>(
      &list.Water_112_42A69B3341E031F505832DA97C2C48EE);
  m_theme.Stamina = *reinterpret_cast<std::array<float, 4> *>(
      &list.Stamina_113_0EB03C574F7FA2A842A3EF821679F5BC);
  m_theme.Energy = *reinterpret_cast<std::array<float, 4> *>(
      &list.Energy_114_0175E3BA4CC7D2B0D2F77BA20F9EC2FD);
  m_theme.XP = *reinterpret_cast<std::array<float, 4> *>(
      &list.XP_115_0D2805024255DAED45A869BC2C547834);
  m_theme.BasicText = *reinterpret_cast<std::array<float, 4> *>(
      &list.BasicText_116_6701154442EC1644EC0B47AE383576F8);
  m_theme.HeaderText = *reinterpret_cast<std::array<float, 4> *>(
      &list.HeaderText_117_9373EE824C65BE0F7EF86E9486BD7211);
  m_theme.SubheaderText = *reinterpret_cast<std::array<float, 4> *>(
      &list.SubheaderText_118_760B5C3048F35745B828B19C0AAC147D);
  m_theme.DisabledText = *reinterpret_cast<std::array<float, 4> *>(
      &list.DisabledText_119_7859802143A6210E3446E3B6DB97EE6A);
  m_theme.GlobalBG = *reinterpret_cast<std::array<float, 4> *>(
      &list.GlobalBG_120_8F619B104D4EF72D4E1828945486C23F);
  m_theme.Augusta_BgBannerDark = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_BgBannerDark_233_8D9815D7444A06C36A61A8BE50345E70);
  m_theme
      .GamepadAttachedBtnBGFadeout = *reinterpret_cast<std::array<float, 4> *>(
      &list.GamepadAttachedBtnBGFadeout_122_E12CA2214DF12A72910F76B87B91EB56);
  m_theme.ButtonBGAccent = *reinterpret_cast<std::array<float, 4> *>(
      &list.ButtonBGAccent_108_57853F7C4686BC108E80D4A6E200D6C3);
  m_theme.Attention = *reinterpret_cast<std::array<float, 4> *>(
      &list.Attention_123_FDE05925442649CA6587DEAE8F9346EF);
  m_theme.LoseResource = *reinterpret_cast<std::array<float, 4> *>(
      &list.LoseResource_128_0DFB47C74E001523CC86DA8DB0B12859);
  m_theme.GainResource = *reinterpret_cast<std::array<float, 4> *>(
      &list.GainResource_129_288801794C058B38FCFB83950C0779F4);
  m_theme.TabButtonCheckedText = *reinterpret_cast<std::array<float, 4> *>(
      &list.TabButtonCheckedText_132_68A2930D44CEB4E1255BAFBA1F03234F);
  m_theme.TabButtonUncheckedText = *reinterpret_cast<std::array<float, 4> *>(
      &list.TabButtonUncheckedText_134_99CE675F42886787C6CF89891631619E);
  m_theme.ButtonUpText = *reinterpret_cast<std::array<float, 4> *>(
      &list.ButtonUpText_136_71C8F17540823CA44BCB64B6B591DAB4);
  m_theme.ButtonOverText = *reinterpret_cast<std::array<float, 4> *>(
      &list.ButtonOverText_138_E3763EFC493818829CAD44B43E846430);
  m_theme.ButtonDisabledText = *reinterpret_cast<std::array<float, 4> *>(
      &list.ButtonDisabledText_140_933091684B348F924F65D8B0DD53F66C);
  m_theme.Durability = *reinterpret_cast<std::array<float, 4> *>(
      &list.Durability_142_2DED48E4490660CFC9977F858D0D8FBA);
  m_theme.Power = *reinterpret_cast<std::array<float, 4> *>(
      &list.Power_144_DFB3DB274BB134C176002EB3321A271D);
  m_theme.MaterialA = *reinterpret_cast<std::array<float, 4> *>(
      &list.MaterialA_147_E5E4A58A401D1BA5492965BB8A23858F);
  m_theme.MaterialB = *reinterpret_cast<std::array<float, 4> *>(
      &list.MaterialB_149_AE40BC8C45CE9C1C78867DA824901292);
  m_theme.BottomButtonOver = *reinterpret_cast<std::array<float, 4> *>(
      &list.BottomButtonOver_153_2AD8E33A445E936CEEA4DF9207F2D22E);
  m_theme.SelectedButtonTextOver = *reinterpret_cast<std::array<float, 4> *>(
      &list.SelectedButtonTextOver_155_32C4FA9441D9128AB124E393B521752D);
  m_theme.Interact = *reinterpret_cast<std::array<float, 4> *>(
      &list.Interact_158_FB1DE29C46A900EF3DA7408226016894);
  m_theme.HighContrastBG = *reinterpret_cast<std::array<float, 4> *>(
      &list.HighContrastBG_161_3F3F3CF242AC12C766A906957EA02876);
  m_theme.HeatMapPositive = *reinterpret_cast<std::array<float, 4> *>(
      &list.HeatMapPositive_166_BCAB72D046B77B7376CD84908EB23AE7);
  m_theme.HeatMapNegative = *reinterpret_cast<std::array<float, 4> *>(
      &list.HeatMapNegative_167_90B1DBD74BFB0A24C8067981068ED85D);
  m_theme.InvisibleElementA = *reinterpret_cast<std::array<float, 4> *>(
      &list.InvisibleElementA_171_B9F1516D448D5A2A74CC16BECB002834);
  m_theme.InvisibleElementB = *reinterpret_cast<std::array<float, 4> *>(
      &list.InvisibleElementB_173_09999EBD4174B5D65623CE96EDADBE4B);
  m_theme.Augusta_InventoryGearText = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_InventoryGearText_247_BAB8BBF94F59DE1A5C64A98EE312B13F);
  m_theme.Augusta_SCABLogoColor = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_SCABLogoColor_189_021EF8374459DAA3128051BE82A18237);
  m_theme.Augusta_SCABTabSelected = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_SCABTabSelected_192_1C576F284CCA08EC48CCFA9020FDA8ED);
  m_theme.Augusta_SCABTabNormal = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_SCABTabNormal_194_4D814AC04BDD144F7223888B6024D56C);
  m_theme.Augusta_StrokeBanner = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_StrokeBanner_199_A3E4AE9E4014596EEEBD0A9659B62D69);
  m_theme.Augusta_StatsBarNormal = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_StatsBarNormal_206_BD6D2BDE41318BB0486D83BDCA2C8072);
  m_theme.Augusta_StatsBarHighlight = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_StatsBarHighlight_204_783890B34BC51A97E6EC5E919A972983);
  m_theme.Augusta_Rarity01 = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_Rarity01_210_CE9C74294935398F094C4E85BEBAE065);
  m_theme.Augusta_Rarity02 = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_Rarity02_214_9CF718354E572C0AB0BB6F9749D2987C);
  m_theme.Augusta_Rarity03 = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_Rarity03_215_C2553246423E899E1EECB3B7DD5735C1);
  m_theme.Augusta_Rarity04 = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_Rarity04_216_27FADB9449E7633149C3F588FB41C332);
  m_theme.Augusta_MainSelectorBg = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MainSelectorBg_219_67FCCB7F4A58C2B01B956B94FCF49925);
  m_theme
      .Augusta_MainSelectorGradient = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MainSelectorGradient_224_4FC704C84D58379DFC40719C79B9BB77);
  m_theme.Augusta_MainSelectorBevel = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MainSelectorBevel_225_FDE3BA9B4778BE7AC819C89978D9E511);
  m_theme
      .Augusta_MainSelectorOutline = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MainSelectorOutline_226_4E6CC8D44EC0D45091FB9F976C61F70F);
  m_theme.Augusta_MainSelectorBlur = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MainSelectorBlur_227_7D43C3A348C86968DAE4EEAA275C8D8C);
  m_theme.Augusta_BgBannerLight = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_BgBannerLight_234_3F169778444E9295EEF49993E1BD7925);
  m_theme.Augusta_BgBannerEffects = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_BgBannerEffects_237_46BBCF5A4DCC314B1886618D7607A9FA);
  m_theme.Augusta_BgTram = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_BgTram_240_9793D9BC4B6DA355DF3A489939EF4E69);
  m_theme
      .Augusta_GlobalPositiveColor = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_GlobalPositiveColor_246_7F30DBE242F7BA8137AD47AADAD13341);
  m_theme
      .Augusta_GlobalNegativeColor = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_GlobalNegativeColor_245_2A0324C34529386D6E2F8BAEA328A1D5);
  m_theme.Augusta_InventoryGearOutlineText = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_InventoryGearOutlineText_250_E9664F6043551A76EE1B8CAFB69A9632);
  m_theme.Augusta_TooltipColor = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_TooltipColor_257_0000B89E4953B110856120849DA5B36F);
  m_theme
      .Augusta_TooltipColorDisabled = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_TooltipColorDisabled_259_5CF6A8F24D59EF35C87187819643B7E6);
  m_theme.Augusta_PersonalMilkMolar = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_PersonalMilkMolar_264_A6A4913E4CB7FABD3289CD9883602651);
  m_theme.Augusta_PartyMilkMolar = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_PartyMilkMolar_265_D9B8062A4AA8D583A54CA6BF764EA67E);
  m_theme.Augusta_OminentMainBG = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentMainBG_268_4AF8D76B4D973057E7A8838DA19FB7E1);
  m_theme.Augusta_OminentBackgroundSlot = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_OminentBackgroundSlot_270_38479D5B434DCF460807C8B2A377EE73);
  m_theme.Augusta_OminentForegroundSlot = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_OminentForegroundSlot_272_6C3B932145F39ED7BF8019BA65FC5C58);
  m_theme.Augusta_OminentBlack = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentBlack_275_F69EDECB4B3D7C7B2FDEDEB962DA6B87);
  m_theme.Augusta_OminentRed = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentRed_276_1D87F3724B787C3590F939B268470CE1);
  m_theme.Augusta_OminentWhite = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentWhite_278_E84C2DC94BE217A60919FCA9698B53E6);
  m_theme.Augusta_OminentSelectorBg = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentSelectorBg_289_BC18383C4E4EE4DE5919B49BF51BBED8);
  m_theme.Augusta_OminentSelectorGradient = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_OminentSelectorGradient_290_1D1EAB024162BDAEC1C64AA74FF9C164);
  m_theme
      .Augusta_OminentSelectorBevel = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentSelectorBevel_291_B1A007E649FF757842CFAB90F8EF586C);
  m_theme.Augusta_OminentSelectorOutline = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_OminentSelectorOutline_292_E96DC360468BAAD385D9A8ACB7BB04C7);
  m_theme
      .Augusta_OminentSelectorBlur = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentSelectorBlur_293_D8806142496659CBA4AFEE9302DC1815);
  m_theme
      .Augusta_OminentBgBannerDark = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentBgBannerDark_298_84F1AC1F416C34384725E2B8257D31B2);
  m_theme
      .Augusta_OminentBgBannerLight = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentBgBannerLight_299_95966CDC47F650AA7F71AA9604D5BD3A);
  m_theme.Augusta_OminentPanelSelectionBg = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_OminentPanelSelectionBg_302_E6F8D9084CDE2A2DC775948C9CFC0E6C);
  m_theme
      .Augusta_OminentButtonHover = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentButtonHover_305_1FCDCA3A4858F4CB40A46D9D9DCFC2C2);
  m_theme.Augusta_PanelSelectionBg = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_PanelSelectionBg_308_D88C1C664F097730943724A55E172A06);
  m_theme.Augusta_MapColorBg = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MapColorBg_312_CA80B53B4D5AB9FAB8627BAD21DAB28F);
  m_theme.Augusta_MapColorMarker = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MapColorMarker_316_6276DAB04B0D7565F44B20AD057F681C);
  m_theme.Augusta_MapColorScanner = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MapColorScanner_319_E86D3B684AB274526496B58C64D7BE19);
  m_theme.Augusta_OminentYellow = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentYellow_322_56EEC97541E9208BA4963E8F6FA0E83B);
  m_theme.Augusta_Subject_V = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_Subject_V_325_1CE1D8434C28149759C94EB50B08D380);
  m_theme.Augusta_OminentInsideSlot = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_OminentInsideSlot_328_4BC13F7245A5C464CBB7B4BE256DFD9D);
  m_theme.Augusta_MetaMenusMainBG = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MetaMenusMainBG_331_6ECF8BAB4CE102D09DA5B09B393719F5);
  m_theme.Augusta_MetaMenusBgBannerLight = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_MetaMenusBgBannerLight_334_0DEAE1DB4E8C6F34B278D58CE543272C);
  m_theme
      .Augusta_MetaMenusBgScrollBar = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MetaMenusBgScrollBar_336_AAB0AF6241840DD9EFD5AEAFBB2E9322);
  m_theme.Augusta_MetaMenusBgBottomButton = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_MetaMenusBgBottomButton_338_338F9D284CA2CCD2276EA385F6682D75);
  m_theme.Augusta_MetaMenusBgBottomButtonOver = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_MetaMenusBgBottomButtonOver_340_028ACE724C99F201E8F48E826A001D0C);
  m_theme.Augusta_MetaMenusBackgroundSlot = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_MetaMenusBackgroundSlot_343_A5AB0BD94A441E3F8F99E3A0D2202D50);
  m_theme.Augusta_MetaMenusForegroundSlot = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_MetaMenusForegroundSlot_345_D962E26B4716308CAA91849F3ABC8C50);
  m_theme.Augusta_MetaMenusBlueSlotBackground = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_MetaMenusBlueSlotBackground_352_66CF2CD8425ECBBDFF53DAA4B01A6701);
  m_theme.Augusta_MetaMenusBlueSlotForeground = *reinterpret_cast<
      std::array<float, 4> *>(
      &list.Augusta_MetaMenusBlueSlotForeground_353_C059BC3D4B7EFCDCCEFEB2BD2F59CAD4);
  m_theme.Augusta_MetaMenusBgPopups = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_MetaMenusBgPopups_356_BAFC76B54454E9F374AD8E980B97CD41);
  m_theme.Augusta_BgPopups = *reinterpret_cast<std::array<float, 4> *>(
      &list.Augusta_BgPopups_358_C52B7AA340A4F68284F32E8815FE655B);
}

static_assert(alignof(SDK::FLinearColor) == 0x000004,
              "Wrong alignment on FLinearColor");
static_assert(sizeof(SDK::FLinearColor) == 0x000010,
              "Wrong size on FLinearColor");
static_assert(offsetof(SDK::FLinearColor, R) == 0x000000,
              "Member 'FLinearColor::R' has a wrong offset!");
static_assert(offsetof(SDK::FLinearColor, G) == 0x000004,
              "Member 'FLinearColor::G' has a wrong offset!");
static_assert(offsetof(SDK::FLinearColor, B) == 0x000008,
              "Member 'FLinearColor::B' has a wrong offset!");
static_assert(offsetof(SDK::FLinearColor, A) == 0x00000C,
              "Member 'FLinearColor::A' has a wrong offset!");
