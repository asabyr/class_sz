import numpy as np
from classy_sz import Class
from scipy.optimize import fsolve
from scipy.interpolate import interp1d
import math
import matplotlib.pyplot as plt
import argparse
import time

import pyccl as ccl




# set path to the class_sz code
path_to_class = '/Users/boris/Work/CLASS-SZ/SO-SZ/class_sz/'
# path_to_class = '/Users/boris/Downloads/class_sz-df37ebccd13a2e266cecd9bf553b729e93101e7e/'
# set path to to the repository containing the folder 'cib_files' with your cib's in it
# set path to where to save the figures
FIG_DIR = '/Users/boris/Work/CLASS-SZ/SO-SZ/figures'
# subprocess.call(['mkdir','-p',path_to_class+'sz_auxiliary_files/run_scripts'])


def run(args):
    # important parameters are re-ajusted later, here we just load a template file:
    # parameter_file = 'class-sz_parameters_MM20.ini'

    cosmology = {
    'h' : 0.6774,
    'Omega_b' : 0.04860,
    'Omega_cdm' : 0.315-0.04860,
    # 'A_s': 2e-9,
    'sigma8':0.831,
    'n_s' : 0.9667,
    # this takes ages:
    # 'N_ncdm': 1,
    # 'N_ur': 2.0328,
    # 'm_ncdm': 0.0
    }

    M = Class()
    M.set(cosmology)
    M.set({'output':"mPk"})
    start = time.time()
    M.compute()
    end = time.time()
    print('class_sz time:',end-start)
    print(M.get_current_derived_parameters(['Neff']))
    print(M.get_current_derived_parameters(['sigma8']))


    start = time.time()
    cosmo = ccl.Cosmology(
        Omega_c=cosmology['Omega_cdm'],
        Omega_b=cosmology['Omega_b'],
        h=cosmology['h'],
        # A_s = cosmology['A_s'],
        sigma8 = cosmology['sigma8'],
        n_s=cosmology['n_s'],
        m_nu=0,
        m_nu_type='normal',
        Neff=M.get_current_derived_parameters(['Neff'])['Neff'],
        transfer_function='boltzmann_class',
                         )
    end = time.time()
    print('ccl cosmo time:',end-start)
    print('ccl sigma8:',ccl.sigma8(cosmo))
    hmd_200c = ccl.halos.MassDef200c()
    hmd_500c = ccl.halos.MassDef500c()
    hmd_200m = ccl.halos.MassDef200m()



    M = Class()
    M.set(cosmology)
    M.set({
    'output':'dndlnM',
    # 'output':'mPk',

    "P_k_max_1/Mpc": 50.,
    # "z_max_pk": 1./0.01-1.,

    "non_linear": "hmcode",

    'mass function':'T08M200c',
    # 'mass function':'T08',
    # 'mass function':'T10',
    # 'mass function':'M500',
    # 'T10_alpha_fixed':1,

    'M_min':1e14,
    'M_max':1e16,
    'ndim_masses':300,
    'n_m_dndlnM':300,



    'z_min':0.,
    'z_max': 3.,
    'n_z_dndlnM':200,
    'ndim_redshifts':200,

    'no_spline_in_tinker' : 1, # ccl doesnt have splines... its interp 1d only.
    # # 'HMF_prescription_NCDM' : 'CDM',
    'HMF_prescription_NCDM' : 'CDM',

    # these params determin how many k's when we compute sigma
    'k_per_decade_class_sz':50.,
    'k_min_for_pk_class_sz':1e-5,
    'k_max_for_pk_class_sz':50.

    })


    start = time.time()
    # hmfs = ccl.halos.MassFuncTinker08(cosmo,mass_def=hmd_200m)
    hmfs = ccl.halos.MassFuncTinker08(cosmo,mass_def=hmd_200c)
    # hmfs = ccl.halos.MassFuncTinker10(cosmo,mass_def=hmd_200m)
    end = time.time()
    print('ccl hmf time:',end-start)


    # zp = np.linspace(0.,3.,100)
    # Masses = np.geomspace(1e14,1e16,300)
    # print(np.vectorize(hmfs.get_mass_function)(cosmo, Masses/cosmology['h'], 1./(1.+zp)))
    # exit(0)



    start = time.time()
    M.compute()
    end = time.time()
    print('class_sz hmf time:',end-start)



    # exit(0)
    #prepare the figure
    label_size = 12
    title_size = 15
    legend_size = 13
    handle_length = 1.5
    fig, (ax1,ax2) = plt.subplots(1,2,figsize=(10,5))
    ax = ax1
    ax.tick_params(axis = 'x',which='both',length=5,direction='in', pad=10)
    ax.tick_params(axis = 'y',which='both',length=5,direction='in', pad=5)
    ax.xaxis.set_ticks_position('both')
    ax.yaxis.set_ticks_position('both')
    plt.setp(ax.get_yticklabels(), rotation='horizontal', fontsize=label_size)
    plt.setp(ax.get_xticklabels(), fontsize=label_size)
    ax.grid(  which="both", alpha=0.3, linestyle='--')

    ax.set_xscale('log')
    ax.set_yscale('linear')
    ax.set_xlabel(r'$m$',size=title_size)



    ax = ax2
    ax.tick_params(axis = 'x',which='both',length=5,direction='in', pad=10)
    ax.tick_params(axis = 'y',which='both',length=5,direction='in', pad=5)
    ax.xaxis.set_ticks_position('both')
    ax.yaxis.set_ticks_position('both')
    plt.setp(ax.get_yticklabels(), rotation='horizontal', fontsize=label_size)
    plt.setp(ax.get_xticklabels(), fontsize=label_size)
    ax.grid(  which="both", alpha=0.3, linestyle='--')

    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_xlabel(r'$m$',size=title_size)





    for zp in [1e-3,0.5,1.,1.5,2.]:
        # zp = 1.5
        dm = hmfs._get_Delta_m(cosmo,1./(1.+zp))
        print('dm',dm)

        om0 = M.get_Omega_m_0()
        or0 = M.get_Omega_r_0()
        ol0 = 1.-om0-or0
        Omega_m_z = om0*(1.+zp)**3./(om0*(1.+zp)**3.+ ol0 + or0*(1.+zp)**4.)
        delta_mean = 500./Omega_m_z;
        print('dmcsz',delta_mean)
        # exit(0)


        Masses = np.geomspace(1e14,1e16,300)
        dndm = 1./Masses*np.vectorize(M.get_dndlnM_at_z_and_M)(zp,Masses)
        sigM = np.vectorize(M.get_sigma_at_z_and_m)(zp,Masses)
        print('class_sz sigM:',sigM)

        # fsigma = hmfs._get_fsigma(cosmo,0.13377606,1./(1.+zp),np.log(5e14))
        # print('fsigma',fsigma)
        # exit(0)

        # _get_fsigma

        nm = hmfs.get_mass_function(cosmo, Masses/cosmology['h'], 1./(1.+zp))


        print('hmf class_sz:',dndm)
        print('hmf ccl:',(nm/np.log(10)/cosmology['h']**3/Masses))
        print('hmf ratio:',dndm/(nm/np.log(10)/cosmology['h']**3/Masses))

        ax1.plot(Masses,100.*(dndm/(nm/np.log(10)/cosmology['h']**3/Masses)-1.), label='class_sz/ccl in percents z=%.2e'%zp)

        ax2.plot(Masses,dndm, label='class_sz z=%.2e'%zp)
        ax2.plot(Masses,(nm/np.log(10)/cosmology['h']**3/Masses), label='ccl z=%.2e'%zp,ls='--')

    ax1.legend()

    ax2.legend()
        #end loop on p over param values

    fig.tight_layout()

    if (args.save_fig == 'yes'):
        FIG_NAME = '/my_figure'
        plt.savefig(FIG_DIR + FIG_NAME +".pdf")

    plt.show(block=True)







def main():
    parser=argparse.ArgumentParser(description="Plot cosmotherm spectra")
    # parser.add_argument("-mode",help="plot or run" ,dest="mode", type=str, required=True)
    # parser.add_argument("-param_name",help="name of varying parameter" ,dest="param_name", type=str, required=True)
    parser.add_argument("-min",help="minimum value of parameter" ,dest="p_min", type=str, required=False)
    parser.add_argument("-max",help="maximum value of parameter" ,dest="p_max", type=str, required=False)
    parser.add_argument("-N",help="number of evaluations" ,dest="N", type=int, required=False)
    parser.add_argument("-p_val",help="list of param values" ,dest="p_val", type=str, required=False)
    parser.add_argument("-spacing",help="linear (lin) or log spacing (log)" ,dest="spacing", type=str, required=False)
    parser.add_argument("-y_min",help="ylim for y-axis" ,dest="y_min", type=str, required=False)
    parser.add_argument("-y_max",help="ylim for y-axis" ,dest="y_max", type=str, required=False)
    parser.add_argument("-x_min",help="xlim for x-axis" ,dest="x_min", type=str, required=False)
    parser.add_argument("-x_max",help="xlim for x-axis" ,dest="x_max", type=str, required=False)
    parser.add_argument("-output",help="what quantities to compute" ,dest="output", type=str, required=False)
    parser.add_argument("-save_figure",help="save figure" ,dest="save_fig", type=str, required=False)
    parser.add_argument("-plot_cib_cib",help="cib_cib" ,dest="plot_cib_cib", type=str, required=False)
    parser.add_argument("-plot_tSZ_cib",help="tSZ_cib" ,dest="plot_tSZ_cib", type=str, required=False)
    parser.add_argument("-plot_lens_cib",help="lens_cib" ,dest="plot_lens_cib", type=str, required=False)
    parser.set_defaults(func=run)
    args=parser.parse_args()
    args.func(args)

if __name__=="__main__":
	main()
