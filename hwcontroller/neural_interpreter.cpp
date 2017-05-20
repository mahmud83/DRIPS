#include "neural_interpreter.h"
#include "common.h"
#include <avr/pgmspace.h>

#define INPUT_SIZE  30
#define OUTPUT_SIZE  8

static const float W[OUTPUT_SIZE][INPUT_SIZE] PROGMEM = {
    {1.414728641510009766e+00,3.179329156875610352e+00,-1.084234952926635742e+00,-2.275654792785644531e+00,-9.747422933578491211e-01,5.239061713218688965e-01,-7.121102809906005859e-01,1.039447069168090820e+00,-1.485677242279052734e+00,-5.718987584114074707e-01,4.072673320770263672e-01,7.121504545211791992e-01,-6.458334326744079590e-01,2.941429853439331055e+00,-9.267437458038330078e-01,2.702958881855010986e-01,5.489435791969299316e-01,2.962597608566284180e-01,4.324054345488548279e-03,-3.506082892417907715e-01,1.097027510404586792e-01,-2.169613689184188843e-01,3.054234087467193604e-01,2.618998587131500244e-01,5.199543833732604980e-01,1.927297748625278473e-02,-4.924572259187698364e-02,-3.091372549533843994e-01,-1.992588676512241364e-02,1.425700426101684570e+00},
    {-3.683581650257110596e-01,-1.590132236480712891e+00,-2.007194012403488159e-01,-2.562036812305450439e-01,5.041136145591735840e-01,-1.380887925624847412e-01,-1.234462022781372070e+00,-6.702239513397216797e-01,5.958885550498962402e-01,4.049415290355682373e-01,3.810369491577148438e+00,-3.538706004619598389e-01,4.705854356288909912e-01,-1.023168325424194336e+00,3.026187419891357422e-01,3.740120828151702881e-01,5.558912158012390137e-01,2.555997073650360107e-01,1.795173287391662598e-01,-5.234802961349487305e-01,4.819222688674926758e-01,4.574939608573913574e-01,3.242148160934448242e-01,-7.167541771195828915e-04,-3.816589713096618652e-01,-5.103841051459312439e-02,7.806727290153503418e-01,1.303448915481567383e+00,2.599167525768280029e-01,-1.607215046882629395e+00},
    {-8.840449452400207520e-01,-8.900513052940368652e-01,1.466342449188232422e+00,-9.446020722389221191e-01,-3.026830255985260010e-01,-3.361341357231140137e-01,3.328500032424926758e+00,1.743198931217193604e-01,2.197875082492828369e-02,-5.877838656306266785e-02,-2.570853471755981445e+00,-1.508509516716003418e-01,2.218364626169204712e-01,-1.586060881614685059e+00,3.993644714355468750e-01,1.507965326309204102e+00,1.395736783742904663e-01,-3.944597244262695312e-01,2.099148780107498169e-01,5.767326429486274719e-02,6.339316368103027344e-01,7.237252593040466309e-01,3.028219044208526611e-01,7.314667701721191406e-01,-6.426733136177062988e-01,1.097175717353820801e+00,8.952419273555278778e-03,6.977035105228424072e-02,3.940244913101196289e-01,5.974148213863372803e-02},
    {2.384606562554836273e-02,-1.674397587776184082e-01,-6.626244634389877319e-03,-1.025536432862281799e-01,-2.328346669673919678e-02,-3.417123481631278992e-02,-1.746533066034317017e-01,-8.806052058935165405e-02,5.060976371169090271e-02,1.305847801268100739e-02,-1.487339138984680176e-01,-6.742592435330152512e-03,7.747422158718109131e-02,-1.906424947082996368e-02,4.373931139707565308e-02,-3.503760397434234619e-01,-1.743903607130050659e-01,-3.058611154556274414e-01,-1.648985594511032104e-01,-2.163328379392623901e-01,-3.195917308330535889e-01,-2.606132328510284424e-01,-2.425416111946105957e-01,-1.375096738338470459e-01,-2.299914360046386719e-01,-2.327482700347900391e-01,-2.587904930114746094e-01,-2.529624402523040771e-01,-1.579685807228088379e-01,-2.233249694108963013e-01},
    {-5.592863261699676514e-02,-1.063618212938308716e-01,-2.327709048986434937e-01,3.935086011886596680e+00,6.424536108970642090e-01,7.202225923538208008e-02,-9.084396958351135254e-01,-3.880479037761688232e-01,8.650547266006469727e-01,2.461626827716827393e-01,-1.020632624626159668e+00,-5.221117660403251648e-02,-2.804470807313919067e-02,-1.229749098420143127e-01,2.123076617717742920e-01,-9.414282441139221191e-01,-5.197684764862060547e-01,8.380426764488220215e-01,2.302358746528625488e-01,1.457965016365051270e+00,2.067625708878040314e-02,5.785476416349411011e-03,-1.443875283002853394e-01,-3.422946482896804810e-02,1.466338872909545898e+00,2.089627832174301147e-01,2.615940868854522705e-01,-1.545836776494979858e-01,8.851721882820129395e-03,8.099864721298217773e-01},
    {-4.674357920885086060e-02,-7.237557321786880493e-02,-2.634942717850208282e-02,-6.246512010693550110e-02,-6.316076219081878662e-02,-5.553577095270156860e-02,-1.169826984405517578e-01,-5.500122532248497009e-02,7.176084816455841064e-02,1.907470822334289551e-02,-1.191233024001121521e-01,4.983831942081451416e-02,-1.934854313731193542e-02,-1.074751019477844238e-01,9.331187233328819275e-03,-3.077588975429534912e-01,-1.664500087499618530e-01,-2.558400928974151611e-01,-2.098759412765502930e-01,-2.365306317806243896e-01,-2.876006066799163818e-01,-2.993930876255035400e-01,-1.559114605188369751e-01,-2.499774396419525146e-01,-1.839059293270111084e-01,-3.365581631660461426e-01,-2.386261075735092163e-01,-2.775511741638183594e-01,-2.202202379703521729e-01,-1.680123805999755859e-01},
    {-3.403173014521598816e-02,-9.589064121246337891e-02,-2.514429204165935516e-02,-6.743667274713516235e-02,-2.091952040791511536e-02,-6.812467239797115326e-03,-1.895870119333267212e-01,-1.218483448028564453e-01,-8.680183440446853638e-03,3.544144565239548683e-03,-1.810536384582519531e-01,2.187049016356468201e-02,3.881339728832244873e-02,-7.587061077356338501e-02,9.515415877103805542e-02,-3.313507437705993652e-01,-2.435307204723358154e-01,-2.868477404117584229e-01,-2.117223143577575684e-01,-1.345417648553848267e-01,-2.743091881275177002e-01,-1.979534178972244263e-01,-2.224130481481552124e-01,-2.181701809167861938e-01,-2.436466515064239502e-01,-2.450696378946304321e-01,-2.404766380786895752e-01,-2.678922116756439209e-01,-1.711366176605224609e-01,-1.615798771381378174e-01},
    {1.103071775287389755e-02,-1.294842660427093506e-01,3.135991096496582031e-02,-9.745160490274429321e-02,3.696527704596519470e-02,-8.232492208480834961e-02,-1.775083392858505249e-01,-1.018335297703742981e-01,-1.360689103603363037e-02,5.209277942776679993e-02,-1.083225756883621216e-01,1.841510087251663208e-02,-1.370667759329080582e-02,-5.893357098102569580e-02,8.135826140642166138e-02,-2.128983139991760254e-01,-2.222674190998077393e-01,-3.364126980304718018e-01,-2.155226171016693115e-01,-2.314391285181045532e-01,-2.896425724029541016e-01,-2.225638478994369507e-01,-2.175269573926925659e-01,-1.931872814893722534e-01,-2.903003692626953125e-01,-2.735246717929840088e-01,-2.739798426628112793e-01,-2.380087971687316895e-01,-2.247742116451263428e-01,-1.800094842910766602e-01},
};

static const float b[OUTPUT_SIZE] PROGMEM = { 4.046787321567535400e-01,3.251186013221740723e-01,6.826617121696472168e-01,-4.575689136981964111e-01,4.672272801399230957e-01,-4.752077758312225342e-01,-4.915966391563415527e-01,-4.553136825561523438e-01 };

/**
 * Calculate ranks from the input vector and store them in the output vector.
 * The computed values are actually the sorted indices, rather than usual ranks.
 *
 * Note that this function only works on arrays of size 5 for efficiency reasons.
 *
 * Example:
 *     d: [ 44,   23,   71,    9,   88 ]
 *   out: [0.75, 0.25, 0.00, 0.50, 1.00]
 */
static inline void ranks5(uint16_t *d_orig, float *out) {
    uint8_t i, j;
    out[0] = 0.0;
    out[1] = 0.25;
    out[2] = 0.50;
    out[3] = 0.75;
    out[4] = 1.0;

    uint16_t d[5];
    memcpy(&d, d_orig, sizeof(uint16_t) * 5);

    for (i = 1; i < 5; i++) {
        uint16_t tmp = d[i];
        float tmpi = out[i];
        for (j = i; j >= 1 && tmp < d[j-1]; j--) {
                d[j] = d[j-1];
                out[j] = out[j-1];
        }
        d[j] = tmp;
        out[j] = tmpi;
    }
}

static inline void preprocess(uint16_t *fhtLeft, uint16_t *fhtFront, uint16_t *fhtRight, float out[INPUT_SIZE]) {
    // Integer vector
    uint16_t val[15];

    val[0] = max(fhtLeft[LED1_BIN], max(fhtLeft[LED1_BIN-1], fhtLeft[LED1_BIN+1]));
    val[1] = max(fhtLeft[LED2_BIN], max(fhtLeft[LED2_BIN-1], fhtLeft[LED2_BIN+1]));
    val[2] = max(fhtLeft[LED3_BIN], max(fhtLeft[LED3_BIN-1], fhtLeft[LED3_BIN+1]));
    val[3] = max(fhtLeft[LED4_BIN], max(fhtLeft[LED4_BIN-1], fhtLeft[LED4_BIN+1]));
    val[4] = max(fhtLeft[LED5_BIN], max(fhtLeft[LED5_BIN-1], fhtLeft[LED5_BIN+1]));
    val[5] = max(fhtFront[LED1_BIN], max(fhtFront[LED1_BIN-1], fhtFront[LED1_BIN+1]));
    val[6] = max(fhtFront[LED2_BIN], max(fhtFront[LED2_BIN-1], fhtFront[LED2_BIN+1]));
    val[7] = max(fhtFront[LED3_BIN], max(fhtFront[LED3_BIN-1], fhtFront[LED3_BIN+1]));
    val[8] = max(fhtFront[LED4_BIN], max(fhtFront[LED4_BIN-1], fhtFront[LED4_BIN+1]));
    val[9] = max(fhtFront[LED5_BIN], max(fhtFront[LED5_BIN-1], fhtFront[LED5_BIN+1]));
    val[10] = max(fhtRight[LED1_BIN], max(fhtRight[LED1_BIN-1], fhtRight[LED1_BIN+1]));
    val[11] = max(fhtRight[LED2_BIN], max(fhtRight[LED2_BIN-1], fhtRight[LED2_BIN+1]));
    val[12] = max(fhtRight[LED3_BIN], max(fhtRight[LED3_BIN-1], fhtRight[LED3_BIN+1]));
    val[13] = max(fhtRight[LED4_BIN], max(fhtRight[LED4_BIN-1], fhtRight[LED4_BIN+1]));
    val[14] = max(fhtRight[LED5_BIN], max(fhtRight[LED5_BIN-1], fhtRight[LED5_BIN+1]));

    // Compute ranks
    ranks5(&(val[0]), &(out[15]));
    ranks5(&(val[5]), &(out[20]));
    ranks5(&(val[10]), &(out[25]));

    // Compute max
    float maxIntensity = val[0];
    for (uint8_t i = 0; i < 15; i++) {
        if (val[i] > maxIntensity) {
            maxIntensity = val[i];
        }
    }

    // Normalize intensity
    for (uint8_t i = 0; i < 15; i++) {
        out[i] = val[i] / maxIntensity;
    }
}

CrossroadStatus neuralInterpretate(uint16_t *fhtLeft, uint16_t *fhtFront, uint16_t *fhtRight) {
    // We make them static so that:
    //  1. we can be more efficient (no stack allocations)
    //  2. as they're big, we're always sure to have enough memory to hold them
    static float x[INPUT_SIZE];
    static float y[OUTPUT_SIZE];
    static float Wrow[INPUT_SIZE];

    // Fill x with the preprocessed input data
    preprocess(fhtLeft, fhtFront, fhtRight, x);

    // Initialize y = b so that we don't have to sum it later
    memcpy_P(y, &b, sizeof(float) * OUTPUT_SIZE);

    // Matrix multiplication
    for (uint8_t i = 0; i < OUTPUT_SIZE; i++) {
        // Get a row from progmem
        memcpy_P(Wrow, &(W[i]), sizeof(float) * INPUT_SIZE);
        for (uint8_t j = 0; j < INPUT_SIZE; j++) {
            y[i] += x[j] * Wrow[j];
        }
    }

    // Interpretate the result
    uint8_t maxIndex = 0;
    for (uint8_t i = 1; i < OUTPUT_SIZE; i++) {
        if (y[i] > y[maxIndex]) {
            maxIndex = i;
        }
    }

    CrossroadStatus result;
    result.left = ((maxIndex >> 2) & 1) == 1;  // maxIndex IN (4, 5, 6, 7) -> 1xx
    result.front = ((maxIndex >> 1) & 1) == 1; // maxIndex IN (2, 3, 6, 7) -> x1x
    result.right = ((maxIndex >> 0) & 1) == 1; // maxIndex IN (1, 3, 5, 7) -> xx1

    return result;
}