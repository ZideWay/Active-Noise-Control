#include "AdaptiveFilter_test.h"
#include <debug_config.h>
#include <stdio.h>
#include <math.h>
static double weights[L] = {0};
static double esti[L] = {0};
static double inBuffer[L] = {0};
static double aulixBuffer[L] = {0};  
static double referFilterBuffer[L] = {0};  
static double filterOutputBuffer[L] = {0};
static double peBuffer[L] = {0};
static double pfBuffer[L] = {0};
static double pw[M] = {
        0.2, -0.001, 0.001, 0.4, 0.9, -0.1, -0.5, -0.03, 0.2, -0.05, 
        0.05, -0.001, 0.01, 0.3, 0.06, -0.5, -0.5, -0.1, 0.4, -0.01, 
        0.01, -0.003, 0.003, 0.4, 0.8, -0.2, -0.5, -0.1, 0.4, -0.05, 
        0.2, -0.4
    };
static AfData g_tAfdata = 
{
    STEPSIZE,
    L,
    0.0,
    inBuffer,
    referFilterBuffer,
    weights,
    filterOutputBuffer
};

static OMData g_tOMdata = 
{
    MUS,
    L,
    0.0,
    aulixBuffer,
    esti,
    peBuffer,
    pfBuffer,
    1.0
};
//static double vs[N] = {0};
/*
static double vs[200] = {
        0.120226027329295, 0.410069155555281, -0.505093513196618, 0.192787815272982,
        0.0712780745188707, -0.292407992391948, -0.0969541236377114, 0.0766131597935023,
        0.800153880770402, 0.619264945822770, -0.301843896012917, 0.678629517722732,
        0.162205315814504, -0.0140994982764700, 0.159821371939074, -0.0458318039438481,
        -0.0277595201634078, 0.333106511692712, 0.315069690183561, 0.316893857362560,
        0.150151323744393, -0.270002284106578, 0.160379438046155, 0.364531692589136,
        0.109319970419333, 0.231364390602020, 0.162536457007892, -0.0678514534976932,
        0.0657116577075720, -0.176041786672094, 0.198651302352396, -0.256492673414098,
        -0.239006700360506, -0.181009410843135, -0.658361953129659, 0.321631611223061,
        0.0727148151863886, -0.168807103980311, 0.306408068512165, -0.382706705716387,
        -0.0228621059633015, -0.0539891998000272, 0.0713767967650106, 0.0699573089426477,
        -0.193393028751188, -0.00671967411068374, -0.0368680695014903, 0.140359616488624,
        0.244461635343907, 0.248041049909115, -0.193118641892629, 0.0172980186445231,
        -0.271484824216513, -0.248986335096083, -0.00153155632392860, 0.342706555370116,
        -0.172102530311772, 0.0830428270734655, -0.0504422058142177, 0.249848428146584,
        -0.243522179560469, 0.00728007030478941, 0.123548797861239, 0.246103926391247,
        0.345296277001064, 0.0192147855163839, -0.333529732916572, -0.165983736793603,
        -0.237376891937554, 0.525578213107402, -0.137652765403674, 0.167275054075152,
        -0.0430260869804620, 0.198699331675574, -0.171025488550458, -0.313556873818003,
        -0.318052925806373, 0.109163476864823, -0.0396622907719365, -0.0438388925963122,
        0.317367397799221, 0.0652002481405353, 0.0442318962247156, 0.355020309299655,
        -0.179884056398197, 0.155769954862580, 0.186731390430808, -0.0544963621031005,
        0.0482252973912026, -0.260690628194947, -0.256690044857704, 0.0234506994133867,
        0.161500911307843, 0.578133419608100, -0.149121287324873, 0.0418884905253193,
        -0.0184463142894240, -0.432237064637980, -0.0981558160019790, -0.401302388727422, 0.187913681115713, -0.198570010228836, 0.0223814378960069, -0.121760370317409, 0.0678693129420711, -0.134237100162978, 0.109559576476246, 0.165326620443618, 0.382789745259816, -0.0434073421988093,
    -0.478150774251258, -0.187737751218856, 0.302896499935403, -0.239741210726308, 0.214875817598139, 0.0277383785402392, 0.321255131144392, -0.438470569565951, -0.0442066672309315, -0.270082461135713,
    0.650250363582615, 0.184524554381211, 0.308347508168889, -0.236616298930801, -0.104785629465656, -0.0609260120872629, 0.245615211375819, -0.0621340530752413, 0.156869438948743, -0.458800072394343,
    -0.0791232648861782, -0.184159545560356, -0.352640670737245, 0.113586585027242, 0.0630535534938992, 0.00748632925772816, -0.298219454159736, 0.252114937847791, 0.0783024966429815, -0.0668731973585563,
    0.00511831325835261, -0.0585839602378807, -0.391359383090793, -0.0638734990326019, -0.185899203408207, -0.218957186235049, -0.258579271135801, -0.119306996630883, -0.447802963960484, 0.215608253490969,
    0.116288973923984, -0.00447836377159960, -0.00777505120111811, -0.178474803225032, 0.227784953851805, -0.0297883339970480, -0.159773801820214, 0.302179044202779, -0.0502603360707994, -0.131710895341252,
    -0.0656853013171519, -0.189602072068047, -0.250468302510234, 0.564830702272008, 0.370180515428338, 0.0687669521527954, -0.281100210727420, -0.193524534867341, -0.0394742279769237, 0.176966011228679,
    -0.297845243239116, -0.520974133892424, -0.324028005279834, 0.0745752893967727, 0.0875093262751530, 0.100998588476114, -0.0291325340858813, 0.0410741305072778, -0.106471051285184, 0.192753892151520,
    -0.304484140145261, 0.101747701998111, -0.189777186667336, -0.0748829960300434, 0.123606113836180, 0.232347733602176, -0.249911607006436, 0.281891857002146, 0.147612493826127, -0.0151751991052101,
    -0.0436527869150554, -0.0486582591255798, -0.0677769245840759, 0.00515315827984649, 0.0114688722268014, 0.184713255259628, 0.341442367159322, 0.104405241786694, -0.0468933280425631, 0.139796813732530
    };
*/



// 可以在程序中使用 vs 数组...

static double dn[DLEN] = {0};
// static double a[N] = {0};


void AdaptiveFilterTestRun(void)
{
    int i;

	// GetInputSignalIntoBuf(a);       /* 得到输入参考信号 */

    convolution(pw, dn);  /* 得到期望信号 */
    
    
    /* 迭代N次 */	
    for(i = 0; i < N; i++)
    {
        //generate_gaussian_noise(VARIANCE);
        AdaptiveFilterRun(sin(2 * PI * F * (i+1) / FS), dn[i], generate_gaussian_noise(VARIANCE), &g_tAfdata, &g_tOMdata);                        
    }
    for(i = 0; i < L; i++)
    {
        //generate_gaussian_noise(VARIANCE);
        DEBUG("g_tAfdata.pWeights[%d] = %.15f\r\n", i, g_tAfdata.pWeights[i]);            
    }
    for(i = 0; i < L; i++)
    {
        //generate_gaussian_noise(VARIANCE);
        DEBUG("g_tOMdata.pEsti[%d] = %.15f\r\n", i, g_tOMdata.pEsti[i]);            
    }
    DEBUG("g_tOMdata.Ru = %.15f\r\n", g_tOMdata.Ru);
    DEBUG("g_tOMdata.Errorf = %.15f\r\n", g_tOMdata.Errorf);
	   

}