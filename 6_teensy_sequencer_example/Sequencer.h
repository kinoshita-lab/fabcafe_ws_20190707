namespace bbb
{

class Sequencer
{
    enum 
    {
        NUM_INSTRUMENT = 4,
        NUM_STEP = 16,
    };

    struct RecordData
    {
        enum {
            SEQ_NONE = -1,
            NOTE_ON = 1,
        };

        int8_t note;
        int8_t parameter1;
        int8_t parameter2;
        int8_t parameter3;
        int8_t parameter4;

        RecordData()
        {
            clear();
        }

        auto clear() -> void {
            note = SEQ_NONE;
            parameter1 = SEQ_NONE;
            parameter2 = SEQ_NONE;
            parameter3 = SEQ_NONE;
            parameter4 = SEQ_NONE;
        }
    };

public:
    Sequencer() {
        for (auto&& parts : recordedData) {
            for (auto&& step : parts) {
                step.clear();
            }
        }

        auto& kick = recordedData[0];
        kick[0].note = kick[4].note = kick[8].note = kick[12].note = 1;
    

        for (auto&& m: muteState) {
            m = false;
        }
    }

    virtual ~Sequencer() {}

    auto nextStep() -> void {
        step++;

        if (step >= 16) {
            step = 0;
        }
    }

    auto clear(const auto partIndex) -> void {
        for (auto&& step : recordedData[partIndex]) {
                step.clear();
        }
    }

    auto rec(const auto index, const auto countRatio) -> void  {
        auto quantizedStep = countRatio > 0.5 ? step + 1 : step;
        quantizedStep = quantizedStep > 16 ? 0 : quantizedStep;

        recordedData[index][quantizedStep].note = RecordData::NOTE_ON;
    }

    auto noteOnStep(const int index) -> bool {        
        const auto& stepData = recordedData[index][step];
        if (muteState[index]) {
            return false;
        }

        return recordedData[index][step].note != RecordData::SEQ_NONE;
    }

    auto toggleMute(const int index) -> void {
        muteState[index] = !muteState[index];
    }



private:
    uint8_t step; // 0 - 15
    RecordData recordedData[NUM_INSTRUMENT][NUM_STEP];
    bool muteState[NUM_INSTRUMENT];
};

};