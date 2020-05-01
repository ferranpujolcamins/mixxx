namespace EffectXml {
const QString FileHeader("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
// Version history:
// 0 (Mixxx 2.1.0): initial support for saving state of effects
const int kXmlSchemaVersion = 0;

const QString Root("MixxxEffects");
const QString SchemaVersion("SchemaVersion");

const QString Rack("Rack");
const QString RackGroup("Group");

const QString ChainPresetList("ChainPresetList");
const QString QuickEffectChainPresets("QuickEffectChains");
const QString ChainPresetName("ChainPresetName");

const QString ChainsRoot("Chains");
const QString Chain("EffectChain");
const QString ChainSuperParameter("SuperParameterValue");
const QString ChainId("Id");
const QString ChainName("Name");
const QString ChainDescription("Description");
const QString ChainMixMode("MixMode");

const QString EffectsRoot("Effects");
const QString Effect("Effect");
const QString EffectMetaParameter("MetaParameterValue");
const QString EffectId("Id");
const QString EffectBackendType("BackendType");

const QString ParametersRoot("Parameters");
const QString Parameter("Parameter");
const QString ParameterId("Id");
const QString ParameterValue("Value");
const QString ParameterLinkType("LinkType");
const QString ParameterLinkInversion("LinkInversion");
const QString ParameterHidden("Hidden");
} // namespace EffectXml
