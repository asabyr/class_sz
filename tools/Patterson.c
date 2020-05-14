//==================================================================================================
// Author Jens Chluba Jan 2010
// Last modification: April 2011
//==================================================================================================

//==================================================================================================
// patterson formulae & integration
// Integral_-1^1 f(x) dx = f(0)+Sum _1^order w[i]*(f(0-x[i])+f(0+x[i]))
//==================================================================================================


#include "stdlib.h"
#include "math.h"
#include "stdio.h"
//# include <iomanip>
#include "string.h"
//# include <vector>

//# include "routines.h"
# include "Patterson.h"

//using namespace std;

//==================================================================================================
//==================================================================================================
static const unsigned short Patterson_iteration[8]={1, 3, 7, 15, 31, 63, 127, 255};

//==================================================================================================
// order 3
//==================================================================================================
static const double Patterson_x3[1]={
    0.7745966692414834
};

static const double Patterson_w3[2]={
    0.8888888888888888,
    0.5555555555555556
};

//==================================================================================================
// order 7
//==================================================================================================
static const double Patterson_x7[2]={
    0.4342437493468025,
    0.9604912687080203
};

static const double Patterson_w7[4]={
    0.4509165386584741, 0.4013974147759622,
    0.2684880898683334, 0.1046562260264673
};

//==================================================================================================
// order 15
//==================================================================================================
static const double Patterson_x15[4]={
    0.2233866864289669, 0.6211029467372264,
    0.888459232872257, 0.993831963212755
};

static const double Patterson_w15[8]={
    0.2255104997982067, 0.2191568584015875, 0.200628529376989, 0.1715119091363914,
    0.1344152552437842, 0.09292719531512454, 0.05160328299707974, 0.01700171962994026
};

//==================================================================================================
// order 31
//==================================================================================================
static const double Patterson_x31[8]={
    0.1124889431331866, 0.3311353932579768, 0.5313197436443756, 0.7024962064915271,
    0.8367259381688688, 0.9296548574297401, 0.9815311495537401, 0.9990981249676676
};

static const double Patterson_w31[16]={
    0.1127552567207687, 0.1119568730209535, 0.1095784210559246, 0.1056698935802348,
    0.1003142786117956, 0.09362710998126447, 0.08575592004999034, 0.07687962049900353,
    0.0672077542959907, 0.05697950949412336, 0.04646289326175799, 0.03595710330712932,
    0.02580759809617665, 0.01644604985438781, 0.008434565739321106, 0.002544780791561875
};

//==================================================================================================
// order 63
//==================================================================================================
static const double Patterson_x63[16]={
    0.05634431304659279, 0.1682352515522075, 0.2777498220218243, 0.3833593241987304,
    0.4836180269458411, 0.5771957100520458, 0.6629096600247806, 0.7397560443526947,
    0.8069405319502176, 0.8639079381936905, 0.9103711569570043, 0.9463428583734029,
    0.9721828747485818, 0.9886847575474295, 0.997206259372222, 0.9998728881203576
};

static const double Patterson_w63[32]={
    0.05637762836038471, 0.0562776998312543, 0.05597843651047632, 0.05548140435655936,
    0.05478921052796287, 0.05390549933526606, 0.05283494679011652, 0.05158325395204846,
    0.05015713930589954, 0.0485643304066732, 0.04681355499062801, 0.0449145316536322,
    0.04287796002500773, 0.04071551011694432, 0.03843981024945553, 0.03606443278078257,
    0.03360387714820773, 0.03107355111168797, 0.02848975474583355, 0.02586967932721475,
    0.02323144663991027, 0.02059423391591271, 0.01797855156812827, 0.0154067504665595,
    0.01290380010035127, 0.01049824690962132, 0.00822300795723593, 0.006115506822117246,
    0.004217630441558855, 0.002579049794685688, 0.001265156556230068, 0.0003632214818455306
};

//==================================================================================================
// order 127
//==================================================================================================
static const double Patterson_x127[32]={
    0.02818464894974569, 0.08445404008371088, 0.1404242331525602, 0.1958975027111002,
    0.2506787303034832, 0.304576441556714, 0.3574038378315322, 0.4089798212298887,
    0.4591300119898323, 0.5076877575337166, 0.5544951326319325, 0.5994039302422429,
    0.6422766425097595, 0.6829874310910792, 0.7214230853700989, 0.7574839663805136,
    0.7910849337998483, 0.8221562543649804, 0.8506444947683502, 0.8765134144847053,
    0.8997448997769401, 0.9203400254700124, 0.9383203977795929, 0.9537300064257611,
    0.9666378515584165, 0.9771415146397057, 0.9853714995985203, 0.9914957211781061,
    0.9957241046984072, 0.9983166353184074, 0.9995987996719107, 0.9999824303548916
};

static const double Patterson_w127[64]={
    0.02818881418019236, 0.0281763190330166, 0.02813884991562715, 0.02807645579381725,
    0.02798921825523816, 0.0278772514766137, 0.02774070217827968, 0.02757974956648187,
    0.02739460526398143, 0.02718551322962479, 0.02695274966763303, 0.02669662292745036,
    0.02641747339505826, 0.0261156733767061, 0.02579162697602423, 0.02544576996546477,
    0.02507856965294977, 0.02469052474448768, 0.0242821652033366, 0.02385405210603854,
    0.02340677749531401, 0.02294096422938775, 0.0224572658268161, 0.02195636630531782,
    0.02143898001250387, 0.02090585144581202, 0.02035775505847216, 0.0197954950480975,
    0.01921990512472777, 0.01863184825613879, 0.01803221639039129, 0.01742193015946417,
    0.01680193857410386, 0.01617321872957772, 0.01553677555584398, 0.01489364166481518,
    0.01424487737291678, 0.01359157100976555, 0.01293483966360737, 0.01227583056008277,
    0.01161572331995513, 0.0109557333878379, 0.01029711695795636, 0.009641177729702537,
    0.008989275784064136, 0.008342838753968157, 0.007703375233279742, 0.007072489995433555,
    0.006451900050175737, 0.00584344987583564, 0.005249123454808859, 0.004671050372114322,
    0.004111503978654693, 0.003572892783517299, 0.003057753410175531, 0.00256876494379402,
    0.002108815245726633, 0.00168114286542147, 0.001289524082610417, 0.0009383698485423815,
    0.0006326073193626335, 0.0003777466463269846, 0.0001807395644453884, 5.053609520786252e-05
};

//==================================================================================================
// order 255
//==================================================================================================
static const double Patterson_x255[64]={
    0.01409388641078246, 0.0422691647653636, 0.07040697604285517, 0.09848239659811921,
    0.126470584372302, 0.1543468114813781, 0.1820864967592522, 0.2096652382431812,
    0.2370588455898297, 0.2642433724109268, 0.2911951485182467, 0.3178908120684767,
    0.344307341599438, 0.3704220879500782, 0.3962128060576159, 0.4216576866261633,
    0.4467353876620285, 0.4714250658716589, 0.4957064079187615, 0.519559661537457,
    0.5429656664983115, 0.5659058854236544, 0.5883624344476626, 0.6103181137151864,
    0.6317564377111943, 0.6526616654100175, 0.6730188302304185, 0.6928137697791147,
    0.712033155362252, 0.7306645212421813, 0.7486962936169366, 0.7661178193037601,
    0.782919394118283, 0.7990922909608414, 0.8146287876551375, 0.8295221946374014,
    0.8437668826727086, 0.8573583108862322, 0.8702930555481139, 0.8825688402473419,
    0.894184568335559, 0.9051403588132616, 0.9154375871557651, 0.9250789329070757,
    0.9340684361577258, 0.942411565191083, 0.9501152975212949, 0.9571882161098609,
    0.9636406215698121, 0.9694846595024592, 0.9747344597524027, 0.9794062816708626,
    0.9835186575786328, 0.987092527954034, 0.9901513704007702, 0.9927213442827886,
    0.994831502800621, 0.9965141459148903, 0.9978053544959573, 0.9987456144680951,
    0.9993803380250236, 0.9997604909244321, 0.9999439962070544, 0.9999975963797485
};

static const double Patterson_w255[128]={
    0.01409440709009618, 0.01409284506916041, 0.0140881595165083, 0.01408035196255366,
    0.01406942495781358, 0.01405538207264996, 0.01403822789690862, 0.01401796803945661,
    0.01399460912761908, 0.01396815880651694, 0.01393862573830685, 0.01390601960132546,
    0.01387035108913984, 0.01383163190950643, 0.01378987478324094, 0.0137450934430019,
    0.01369730263199072, 0.01364651810257129, 0.0135927566148124, 0.01353603593495621,
    0.01347637483381652, 0.0134137930851101, 0.01334831146372518, 0.01327995174393053,
    0.01320873669752913, 0.01313469009196015, 0.01305783668835305, 0.0129782022395374,
    0.01289581348801211, 0.01281069816387736, 0.01272288498273238, 0.01263240364354208,
    0.01253928482647488, 0.01244356019071404, 0.01234526237224384, 0.01224442498161199,
    0.0121410826016683, 0.01203527078527956, 0.01192702605301927, 0.01181638589083024,
    0.011703388747657, 0.01158807403304395, 0.01147048211469387, 0.0113506543159806,
    0.01122863291340805, 0.01110446113400693, 0.01097818315265891, 0.01084984408933731,
    0.01071949000625193, 0.0105871679048852, 0.01045292572290601, 0.01031681233094762,
    0.01017887752923608, 0.01003917204405684, 0.009897747524048749, 0.009754656536317411,
    0.009609952562363883, 0.009463689993830066, 0.009315924128069395, 0.009166711163560789,
    0.009016108195195643, 0.008864173209482495, 0.008710965079732087, 0.008556543561307689,
    0.008400969287051932, 0.008244303763032867, 0.00808660936478886, 0.007927949334294849,
    0.007768387777921991, 0.007607989665719056, 0.007446820832407591, 0.007284947980553807,
    0.007122438686458388, 0.006959361409390423, 0.006795785504882773, 0.006631781242901888,
    0.006467419831803687, 0.006302773449085758, 0.006137915280041385, 0.005972919565508166,
    0.005807861659977567, 0.005642818101384444, 0.005477866693918951, 0.005313086605187057,
    0.005148558478978178, 0.004984364564765539, 0.004820588864851269, 0.004657317299756855,
    0.004494637892032068, 0.004332640968092983, 0.004171419376984078, 0.004011068724075023,
    0.003851687616639871, 0.003693377917025651, 0.003536244997716778, 0.00338039799108692,
    0.003225950025087868, 0.003073018434702578, 0.00292172493791782, 0.002772195764593451,
    0.00262456172740443, 0.002478958226657568, 0.002335525186057161, 0.002194406925363839,
    0.002055751989327346, 0.001919712971013872, 0.00178644639175865, 0.001656112728154453,
    0.001528876705087765, 0.001404907995655145, 0.00128438247189701, 0.001167484117429959,
    0.001054407622863317, 0.0009453615168585254, 0.0008405714327107224, 0.0007402828042445033,
    0.0006447620413057248, 0.0005542953149303748, 0.0004691849242478504, 0.0003897452844732823,
    0.0003163036608222645, 0.0002492124004829973, 0.0001888732645065049, 0.0001357549109492287,
    9.037273465875115e-05, 5.327529366978061e-05, 2.515787038428066e-05, 6.937936432410826e-06
};

//==================================================================================================
//
// Functions to compute the integral (with external parameter)
//
//==================================================================================================
int compute_integral_function_Patterson(int order, double xc, double Dx,
                                        double (*f)(double,
                                                    void *p),
                                          int *neval, double *r,
                                        double * Patterson_fvals, int *Patterson_fvals_size, void *p)
{
    //==============================================================================================
    *neval=2*order-1;
    *r=0.0;

  //  printf("begin of compute_integral_function_Patterson\n");

    //==============================================================================================
    // local variables
    //==============================================================================================
    const double *x, *w;
    double Del, dum;
    int nx;
    

    //==============================================================================================
    // decide which rule to use
    //==============================================================================================
    if(order==3)       { x=Patterson_x3;   w=Patterson_w3;   nx=1; }
    else if(order==7)  { x=Patterson_x7;   w=Patterson_w7;   nx=2; }
    else if(order==15) { x=Patterson_x15;  w=Patterson_w15;  nx=4; }
    else if(order==31) { x=Patterson_x31;  w=Patterson_w31;  nx=8; }
    else if(order==63) { x=Patterson_x63;  w=Patterson_w63;  nx=16; }
    else if(order==127){ x=Patterson_x127; w=Patterson_w127; nx=32; }
    else if(order==255){ x=Patterson_x255; w=Patterson_w255; nx=64; }
    //==============================================================================================
    else
    {
        printf(" compute_integral_function_Patterson:: too many points requested \n");
        *neval=0;
        return 1;
    }

    //==============================================================================================
    // allocate new memory
    //==============================================================================================
    //vector<double> loc_vals(Patterson_fvals.size()+nx, 0.0);

    double * loc_vals;
    //loc_vals = (double *)malloc((*Patterson_fvals_size + nx)*sizeof(double));
    class_alloc(loc_vals,(*Patterson_fvals_size + nx)*sizeof(double),NULL);


    //printf("after alloc loc_vals, Patterson_fvals_size = %d\n",*Patterson_fvals_size);
    //printf("loc_vals size = %d\n",*Patterson_fvals_size + nx);
    //==============================================================================================
    // evalutation of function at given abscissae
    //==============================================================================================
    for(int k=0; k<nx; k++)
    {
        //printf("inside k loop 0 k=%d, nx=%d, %e\n",k,nx,Patterson_fvals[k]);
        loc_vals[2*k]= Patterson_fvals[k];
        //printf("inside k loop \n");

        //------------------------------------------------------------------------------------------
        // calc new point
        //------------------------------------------------------------------------------------------
        Del=Dx*x[k];
        dum=f(xc+Del, p)+f(xc-Del, p);
        //printf("inside k loop 2\n");
        loc_vals[2*k+1]=dum;
        //printf("inside k loop 3\n");
    }

    //printf("outside k loop \n");
    //==============================================================================================
    // save things for next call
    //==============================================================================================
    //Patterson_fvals.clear();
    //Patterson_fvals=loc_vals;
    //free(Patterson_fvals);
    *Patterson_fvals_size += nx;
    //Patterson_fvals = (double *)malloc(*Patterson_fvals_size*sizeof(double));
    //Patterson_fvals = loc_vals;
    //printf("before realloc \n");
    //class_realloc(Patterson_fvals,Patterson_fvals,*Patterson_fvals_size*sizeof(double),NULL);
    //printf("after realloc \n");
    for(int k=0; k<*Patterson_fvals_size; k++)
    Patterson_fvals[k] = loc_vals[k];
    //==============================================================================================
    // compute integral; start with smallest terms first
    //==============================================================================================
    //for(int k=(int)Patterson_fvals_size-1; k>=0; k--) *r+=Patterson_fvals[k]*w[k];
    //for(int k=(int)Patterson_fvals_size-1; k>=0; k--) *r+=*Patterson_fvals[k]*w[k];
    //for(int k=(int)Patterson_fvals.size()-1; k>=0; k--) *r+=Patterson_fvals[k]*w[k];

    (*r)+=DC_sumprod(&Patterson_fvals[0], w, *Patterson_fvals_size);
    (*r)*=Dx;

    //printf("in compute_integral_function_Patterson r=%e, Patterson_fvals_size = %d\n", (*r),*Patterson_fvals_size);

    //==============================================================================================
    // clean up
    //==============================================================================================
    free(loc_vals);

    return 0;
}

//==================================================================================================
int Integrate_using_Patterson(double a, double b, double epsrel, double epsabs,
                              double (*fptr)(double, void *p), int *neval, double *r, void *p,
                              double * Patterson_fvals, int Patterson_fvals_size)
{
    *r=0.0;
    if(a>=b) return 2;

    double xc=(a+b)/2.0;
    double Dx=(b-a)/2.0;
    int ifail=1;

    double fi=fptr(xc, p);
    //vector<double> Patterson_fvals(1, fi);
    //double * Patterson_fvals;
    //int Patterson_fvals_size =1;
    //Patterson_fvals = (double *) malloc(Patterson_fvals_size*sizeof(double));
    Patterson_fvals[0] = fi;

    //printf("Patterson_fvals = %e\n", Patterson_fvals[0]);

    //----------------------------------------------------------------------------------------------
    // Integral for 1-point formula
    //----------------------------------------------------------------------------------------------
    *r=2.0*fi*Dx;
    *neval=1;

    double r1, Dr;
    //
    for(int it=1; it<8; it++)
    {

        //printf("inside it loop\n");
        ifail=compute_integral_function_Patterson(Patterson_iteration[it], xc, Dx, fptr,
                                                  neval, &r1, Patterson_fvals,&Patterson_fvals_size, p);

        Dr= *r-r1;
        *r=r1;
        //printf("inside it loop Patterson_fvals_size=%d, r=%e\n",Patterson_fvals_size,*r);

        //==========================================================================================
        // check error
        //==========================================================================================
        if(fabs(Dr)<=fmax(epsabs, fabs( *r)*epsrel)) return ifail;
    }

    return ifail;
}
//==================================================================================================
//
// Integral using Patterson formulae (refinement of regions)
//
//==================================================================================================
int Integrate_Patterson_refine(int reclev, double a, double b, double epsrel, double epsabs,
                               double (*fptr)(double, void *p), double *r, void *p,
                               double * Patterson_fvals, int Patterson_fvals_size)
{
    *r=0.0;

    //----------------------------------------------------------------------------------------------
    // to avoid SEGfault for strange integrand
    //----------------------------------------------------------------------------------------------
    if(reclev>=100)
    {
        printf(" Integrate_Patterson_refine::recursion_level has exceeded limit of 100.\n");
        printf(" Exiting \n");
        exit(0);
    }

    int neval, ntot=0, ifail=0;

    int subint=2;
    double a1, b1=a, fa=(b-a)/subint, err;

    for(int k=1; k<=subint; k++)
    {
        a1=b1;
        b1=a1+fa;
        double r1=0.0;
        err=fmax(epsabs, fabs( *r)*epsrel);

        ifail=Integrate_using_Patterson(a1, b1, epsrel, err, fptr, &neval, &r1, p,Patterson_fvals,Patterson_fvals_size);
        ntot+=neval;

        //==========================================================================================
        // do subdivision (recursive calling)
        //==========================================================================================
        if(ifail!=0)
        {
            neval=Integrate_Patterson_refine(reclev, a1, b1, epsrel, err, fptr, &r1, p, Patterson_fvals,Patterson_fvals_size);
            ifail=0;
            ntot+=neval;
        }

        *r+=r1;
    }

    return ntot;
}

//==================================================================================================
double Integrate_using_Patterson_adaptive(double a, double b, double epsrel, double epsabs,
                                          double (*fptr)(double, void *p), void *p, int show_neval)
{
    if(a>=b) return 0.0;


    int neval=0, ntot=0, ifail=0;
    double r;

    double * Patterson_fvals;
    int Patterson_fvals_size = 1;
    int Patterson_fvals_size_max = 256;
    class_alloc(Patterson_fvals,Patterson_fvals_size_max*sizeof(double),NULL);
    for(int k=0; k<Patterson_fvals_size_max; k++)
    Patterson_fvals[k]=0.;


    //printf("before subdiv before integrate\n");
    ifail=Integrate_using_Patterson(a, b, epsrel, epsabs, fptr, &neval, &r, p, Patterson_fvals, Patterson_fvals_size);
    ntot+=neval;
    //printf("before subdiv\n");


    //==========================================================================================
    // do subdivision
    //==========================================================================================
    if(ifail!=0)
    {
        int reclev=0;
        neval=Integrate_Patterson_refine(reclev, a, b, epsrel, epsabs, fptr, &r, p, Patterson_fvals, Patterson_fvals_size);
        ifail=0;
        ntot+=neval;
        //printf("inside subdiv\n");
    }

    if(show_neval) printf(" # of evals = %d\n",ntot);
    free(Patterson_fvals);

    return r;
}

//==================================================================================================
//==================================================================================================

//BB: below is from JC's routines.cpp
//======================================================================================
// divide and conquer sum of product
//======================================================================================
double DC_sumprod(const double *yarr, const double *zarr, int M)
{
    if(M<=4)
    {
        double r=yarr[0]*zarr[0];
        for(int i=1; i<M; i++) r+=yarr[i]*zarr[i];
        return r;
    }

    int N=floor(M/2);
    return DC_sumprod(&yarr[0], &zarr[0], N)+DC_sumprod(&yarr[N], &zarr[N], M-N);
}
