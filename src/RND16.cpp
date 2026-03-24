#include "plugin.hpp"

// Voltage range presets
struct VoltageRange {
    std::string name;
    float min;
    float max;
};

static const std::vector<VoltageRange> voltageRanges = {
    {"0V to +10V", 0.f, 10.f},
    {"-10V to +10V", -10.f, 10.f},
    {"-5V to +5V", -5.f, 5.f},
    {"0V to +5V", 0.f, 5.f},
    {"0V to +1V", 0.f, 1.f},
    {"-1V to +1V", -1.f, 1.f},
    {"0V to +2V", 0.f, 2.f},
    {"0V to +3V", 0.f, 3.f},
    {"0V to +8V", 0.f, 8.f},
};

struct RND16 : Module {
    enum ParamId {
        PARAMS_LEN
    };
    enum InputId {
        INPUT_1, INPUT_2, INPUT_3, INPUT_4, INPUT_5, INPUT_6, INPUT_7, INPUT_8,
        INPUT_9, INPUT_10, INPUT_11, INPUT_12, INPUT_13, INPUT_14, INPUT_15, INPUT_16,
        INPUTS_LEN
    };
    enum OutputId {
        OUTPUT_1, OUTPUT_2, OUTPUT_3, OUTPUT_4, OUTPUT_5, OUTPUT_6, OUTPUT_7, OUTPUT_8,
        OUTPUT_9, OUTPUT_10, OUTPUT_11, OUTPUT_12, OUTPUT_13, OUTPUT_14, OUTPUT_15, OUTPUT_16,
        OUTPUTS_LEN
    };
    enum LightId {
        LIGHTS_LEN
    };

    int rangeIndex[16] = {};
    float outputVoltage[16] = {};
    bool triggerState[16] = {};

    RND16() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        for (int i = 0; i < 16; i++) {
            configInput(INPUT_1 + i, string::f("Trigger %d", i + 1));
            configOutput(OUTPUT_1 + i, string::f("Random %d", i + 1));
            rangeIndex[i] = 0;
        }
    }

    void process(const ProcessArgs& args) override {
        for (int i = 0; i < 16; i++) {
            float triggerIn = inputs[INPUT_1 + i].getVoltage();
            bool triggerHigh = triggerIn >= 1.f;

            if (triggerHigh && !triggerState[i]) {
                const VoltageRange& range = voltageRanges[rangeIndex[i]];
                float randomValue = random::uniform();
                outputVoltage[i] = range.min + randomValue * (range.max - range.min);
            }

            triggerState[i] = triggerHigh;
            outputs[OUTPUT_1 + i].setVoltage(outputVoltage[i]);
        }
    }

    json_t* dataToJson() override {
        json_t* rootJ = json_object();
        json_t* rangesJ = json_array();
        for (int i = 0; i < 16; i++) {
            json_array_append_new(rangesJ, json_integer(rangeIndex[i]));
        }
        json_object_set_new(rootJ, "ranges", rangesJ);

        json_t* voltagesJ = json_array();
        for (int i = 0; i < 16; i++) {
            json_array_append_new(voltagesJ, json_real(outputVoltage[i]));
        }
        json_object_set_new(rootJ, "voltages", voltagesJ);
        return rootJ;
    }

    void dataFromJson(json_t* rootJ) override {
        json_t* rangesJ = json_object_get(rootJ, "ranges");
        if (rangesJ) {
            size_t i;
            json_t* valueJ;
            json_array_foreach(rangesJ, i, valueJ) {
                if (i < 16) rangeIndex[i] = json_integer_value(valueJ);
            }
        }

        json_t* voltagesJ = json_object_get(rootJ, "voltages");
        if (voltagesJ) {
            size_t i;
            json_t* valueJ;
            json_array_foreach(voltagesJ, i, valueJ) {
                if (i < 16) outputVoltage[i] = json_number_value(valueJ);
            }
        }
    }
};

struct RND16Widget : ModuleWidget {
    RND16Widget(RND16* module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/RND16.svg")));

        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        const float panelWidth = 3 * RACK_GRID_WIDTH;
        const float marginX = 2.f;
        const float inputX = marginX + 2.f;
        const float outputX = panelWidth - marginX - 2.f - 7.5f;

        const float topMargin = 20.f;
        const float bottomMargin = 15.f;
        const float usableHeight = RACK_GRID_HEIGHT - topMargin - bottomMargin;
        const float portSpacing = usableHeight / 16;

        for (int i = 0; i < 16; i++) {
            float y = topMargin + portSpacing * (i + 0.5f) - 3.75f;
            addInput(createInput<PJ301MPort>(Vec(inputX, y), module, RND16::INPUT_1 + i));
            addOutput(createOutput<PJ301MPort>(Vec(outputX, y), module, RND16::OUTPUT_1 + i));
        }
    }

    void appendContextMenu(Menu* menu) override {
        RND16* module = getModule<RND16>();

        menu->addChild(new MenuSeparator);
        menu->addChild(createMenuLabel("Output Voltage Ranges"));

        for (int i = 0; i < 16; i++) {
            std::string label = string::f("Output %d Range", i + 1);
            menu->addChild(createIndexSubmenuItem(label,
                []() {
                    std::vector<std::string> rangeNames;
                    for (const auto& range : voltageRanges) {
                        rangeNames.push_back(range.name);
                    }
                    return rangeNames;
                }(),
                [=]() { return module->rangeIndex[i]; },
                [=](int index) { module->rangeIndex[i] = index; }
            ));
        }

        menu->addChild(new MenuSeparator);
        menu->addChild(createMenuLabel("Quick Set All"));

        for (size_t r = 0; r < voltageRanges.size(); r++) {
            menu->addChild(createMenuItem("Set All: " + voltageRanges[r].name, "",
                [=]() {
                    for (int i = 0; i < 16; i++) {
                        module->rangeIndex[i] = r;
                    }
                }
            ));
        }
    }
};

Model* modelRND16 = createModel<RND16, RND16Widget>("RND16");
