/* Copyright 1990-2017 The MathWorks, Inc. */
/*
 * @file: RTWCG_FMU2_target.h
 *  
 * @brief fmustruct 
 *
 * Abstract:
 *      FMI 2.0 data types, function signatures and FMU2_CS_RTWCG(struct)
 *
 *      FMU2_CS_RTWCG is a data struct to store FMU info, handle all 
 *      dynamic function calls to FMU lib 
 *      
 * =============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <simstruc.h>
#include <slsv_diagnostic_codegen_c_api.h>
#include "fmi2Functions.h"

/*raccel_utils.h is needed by win plateform to mute warning warning C4013*/
#include "rapid/raccel_utils.h"

#ifdef _WIN32
#include "windows.h"
#define FMUHANDLE                      HMODULE
#define LOAD_LIBRARY(LIBRARY_LOC)      LoadLibraryEx(LIBRARY_LOC, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#define LOAD_FUNCTION                  GetProcAddress
#define CLOSE_LIBRARY                  FreeLibrary

#else
#include <dlfcn.h>
#define FMUHANDLE                      void*
#define LOAD_LIBRARY(LIBRARY_LOC)      dlopen(LIBRARY_LOC, RTLD_NOW | RTLD_LOCAL)
#define LOAD_FUNCTION                  dlsym
#define CLOSE_LIBRARY                  dlclose
#endif

#define FULL_FCN_NAME_MAX_LEN 256
#define FCN_NAME_MAX_LEN      28

#ifndef getgblSetParamPktReceived_GUARD
#define getgblSetParamPktReceived_GUARD
#define getgblSetParamPktReceived(ptr)       {  \
    extern boolean_T   gblSetParamPktReceived;  \
    *ptr = gblSetParamPktReceived;              \
    }
#endif

#ifndef isSimTargetSimulating_GUARD
#define isSimTargetSimulating_GUARD
#define isSimTargetSimulating(S)                \
    ssGetSimMode((SimStruct*)(S))==SS_SIMMODE_NORMAL || ssRTWGenIsAccelerator((SimStruct*)(S))
#endif

#ifndef ssGetMdlSfuncS_GUARD
#define ssGetMdlSfuncS_GUARD
#define ssGetMdlSfuncS(S)                \
    (S)->_mdlRefSfcnS
#endif

#ifndef RTWCG_FMU2_GUARD
#define RTWCG_FMU2_GUARD
typedef fmi2Status (*_fmi2_default_fcn_type) (fmi2Component, ...);

struct FMU2_CS_RTWCG {
    /*common functions*/
    fmi2GetTypesPlatformTYPE* getTypesPlatform;
    fmi2GetVersionTYPE* getVersion;
    fmi2SetDebugLoggingTYPE* setDebugLogging;
    fmi2InstantiateTYPE* instantiate;
    fmi2FreeInstanceTYPE* freeInstance;
    fmi2SetupExperimentTYPE* setupExperiment;
    fmi2EnterInitializationModeTYPE* enterInitializationMode;
    fmi2ExitInitializationModeTYPE* exitInitializationMode;
    fmi2TerminateTYPE* terminate;
    fmi2ResetTYPE* reset;

    fmi2GetRealTYPE* getReal;
    fmi2GetIntegerTYPE* getInteger;
    fmi2GetBooleanTYPE* getBoolean;
    fmi2GetStringTYPE* getString;
    fmi2SetRealTYPE* setReal;
    fmi2SetIntegerTYPE* setInteger;
    fmi2SetBooleanTYPE* setBoolean;
    fmi2SetStringTYPE* setString;

    fmi2GetFMUstateTYPE* getFMUstate;
    fmi2SetFMUstateTYPE* setFMUstate;
    fmi2FreeFMUstateTYPE* freeFMUstate;
    fmi2SerializedFMUstateSizeTYPE* serializedFMUstateSize;
    fmi2SerializeFMUstateTYPE* serializeFMUstate;
    fmi2DeSerializeFMUstateTYPE* deSerializeFMUstate;
    fmi2GetDirectionalDerivativeTYPE* getDirectionalDerivative;

    /*fmi cs functions*/
    fmi2SetRealInputDerivativesTYPE* setRealInputDerivatives;
    fmi2GetRealOutputDerivativesTYPE* getRealOutputDerivatives;
    fmi2DoStepTYPE* doStep;
    fmi2CancelStepTYPE* cancelStep;
    fmi2GetStatusTYPE* getStatus;
    fmi2GetRealStatusTYPE* getRealStatus;
    fmi2GetIntegerStatusTYPE* getIntegerStatus;
    fmi2GetBooleanStatusTYPE* getBooleanStatus;
    fmi2GetStringStatusTYPE* getStringStatus;

    /*fmi me functions*/
    fmi2EnterEventModeTYPE*               enterEventMode;
    fmi2NewDiscreteStatesTYPE*            newDiscreteStates;
    fmi2EnterContinuousTimeModeTYPE*      enterContinuousTimeMode;
    fmi2CompletedIntegratorStepTYPE*      completedIntegratorStep;
    fmi2SetTimeTYPE*             setTime;
    fmi2SetContinuousStatesTYPE* setContinuousStates;
    fmi2GetDerivativesTYPE*                getDerivatives;
    fmi2GetEventIndicatorsTYPE*            getEventIndicators;
    fmi2GetContinuousStatesTYPE*           getContinuousStates;
    fmi2GetNominalsOfContinuousStatesTYPE* getNominalsOfContinuousStates;

    char * fmuname;
    char * dllfile;
    FMUHANDLE Handle;
    fmi2Component mFMIComp;
    SimStruct *ssPtr;
    fmi2Status FMUErrorStatus;

    fmi2EventInfo eventInfo;
    
    /*two int arrays for maping enum param original value to actual value*/
    int * paramIdxToOffset;
    int * enumValueList;
};

void fmu2Logger(fmi2Component c, fmi2String instanceName, fmi2Status status,
                      fmi2String category, fmi2String message, ...);
void * FMU2_fmuInitializeCS( const char * lib, fmi2String instanceName, fmi2String fmuGUID, fmi2String fmuLocation, void* ssPtr);
fmi2Status FMU2_setupExperiment(void** fmuv, fmi2Boolean isToleranceUsed, fmi2Real toleranceValue, fmi2Real currentTime, fmi2Boolean isTFinalUsed, fmi2Real TFinal);

fmi2Status  FMU2_doStep(void **fmuv, double currentCommunicationPoint,double communicationStepSize, fmi2Boolean noSetFMUStatePriorToCurrentPoint);

fmi2Boolean FMU2_terminate(void **fmuv);
fmi2Boolean FMU2_freeInstance(void **fmuv);
fmi2Boolean FMU2_enterInitializationMode(void** );
fmi2Boolean FMU2_exitInitializationMode(void**);

fmi2Status FMU2_setRealVal(void **fmuv, const fmi2ValueReference dvr, size_t nvr, const fmi2Real dvalue);
fmi2Status FMU2_setReal(void **fmuv, const fmi2ValueReference dvr, size_t nvr, const fmi2Real value[]);
fmi2Status FMU2_getReal(void **fmuv, const fmi2ValueReference dvr, size_t nvr, fmi2Real value[]);
fmi2Status FMU2_setIntegerVal(void **fmuv, const fmi2ValueReference dvr, size_t nvr, const fmi2Integer dvalue);
fmi2Status FMU2_setInteger(void **fmuv, const fmi2ValueReference dvr, size_t nvr, const fmi2Integer value[]);
fmi2Status FMU2_getInteger(void **fmuv, const fmi2ValueReference vr, size_t nvr, fmi2Integer value[]);
fmi2Status FMU2_setBooleanVal(void **fmuv, const fmi2ValueReference dvr, size_t nvr, const fmi2Boolean dvalue);
fmi2Status FMU2_setBoolean(void **fmuv, const fmi2ValueReference dvr, size_t nvr, const fmi2Boolean value[]);
fmi2Status FMU2_getBoolean(void **fmuv, const fmi2ValueReference dvr, size_t nvr, fmi2Boolean value[]);

fmi2Status FMU2_setString(void **fmuv, const fmi2ValueReference dvr, size_t nvr, const fmi2String value[]);
fmi2Status FMU2_setStringVal(void **fmuv, const fmi2ValueReference dvr, size_t nvr, const fmi2String);
fmi2Status FMU2_getString(void **fmuv, const fmi2ValueReference vr, size_t nvr, fmi2String value[]);

void * FMU2_fmuInitialize( const char * lib, fmi2String instanceName, fmi2String fmuGUID, fmi2String fmuLocation, fmi2Type fmuType, void* ssPtr);



/*helper functions to load fmu functions*/
void LoadFMU2CommonFunctions(struct FMU2_CS_RTWCG* fmustruct);
void LoadFMU2CSFunctions(struct FMU2_CS_RTWCG* fmustruct);
void LoadFMU2MEFunctions(struct FMU2_CS_RTWCG* fmustruct);

/*helper to preprocess Enum type*/
void createParamIdxToOffset(void** fmuv, int array_size);
void createEnumValueList(void** fmuv, int array_size);
int setParamIdxToOffsetByIdx(void** fmuv, int idx, int value);
int setEnumValueListByIdx(void** fmuv, int idx, int value);
int getParamIdxToOffsetByIdx(void** fmuv, int idx);
int getEnumValueByIdx(void** fmuv, int idx);

/* me helper functions*/
void FMU2_simTerminate(void **fmuv, const char* blkPath, fmi2Real time);
void FMU2_terminateModel(void **fmuv);
void FMU2_getNextEventTime(void **fmuv, fmi2Real* nextEventTime, int32_T* upcomingTimeEvent);
void * FMU2_fmuInitializeME( const char * lib, fmi2String instanceName, fmi2String fmuGUID, fmi2String fmuLocation, void* ssPtr);
void FMU2_eventIteration(void **fmuv,  const char* blkPath, fmi2Real time);
int FMU2_valuesOfContinuousStatesChanged(void **fmuv);


/*me standard functions wrapper*/
fmi2Status FMU2_enterEventMode             (void** fmuv);
fmi2Status FMU2_enterContinuousTimeMode    (void** fmuv);
fmi2Status FMU2_completedIntegratorStep    (void** fmuv, fmi2Boolean, fmi2Boolean*, fmi2Boolean*);
/* Providing independent variables and re-initialization of caching */
fmi2Status FMU2_setTime                (void** fmuv, fmi2Real);
fmi2Status FMU2_setContinuousStates    (void** fmuv, const fmi2Real[], size_t);
/* Evaluation of the model equations */
fmi2Status FMU2_getDerivatives                   (void** fmuv, fmi2Real[], size_t);
fmi2Status FMU2_getEventIndicators               (void** fmuv, fmi2Real[], size_t);
fmi2Status FMU2_getContinuousStates              (void** fmuv, fmi2Real[], size_t);
fmi2Status FMU2_getNominalsOfContinuousStates    (void** fmuv, fmi2Real[], size_t);

#endif
