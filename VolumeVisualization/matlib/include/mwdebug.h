/*
 * @(#)mwdebug.h    generated by: makeheader 4.21  Tue Jul 26 23:02:50 2005
 *
 *		built from:	../../src/include/copyright.h
 *				../src/mwdebug.c
 */

#ifndef mwdebug_h
#define mwdebug_h


/*
 * Copyright 1984-2003 The MathWorks, Inc.
 * All Rights Reserved.
 */


#ifdef __cplusplus
    extern "C" {
#endif


#include "matrix.h"


#include "mat.h"


extern int matClose_d(MATFile *ph, const char * file, int linenum);


extern int matDeleteVariable_d(MATFile *ph, const char *name, const char * file, int linenum);


extern mxArray * matGetVariable_d(MATFile *ph, const char *name, const char * file, int linenum);


extern mxArray *matGetVariableInfo_d(MATFile *ph, const char *name, const char * file, int linenum);


extern const char **matGetDir_d(MATFile *ph, int *num, const char * file, int linenum);


extern FILE *matGetFp_d(MATFile *ph, const char * file, int linenum);


extern mxArray *matGetNextVariable_d(MATFile *ph, const char **pname, const char * file, int linenum);


extern mxArray *matGetNextVariableInfo_d(MATFile *ph, const char **pname, const char * file, int linenum);


extern MATFile *matOpen_d(const char *filename, const char *mode, const char * file, int linenum);


extern int matPutVariable_d(MATFile *ph, const char *name, const mxArray *mp, const char * file, int linenum);


extern int matPutVariableAsGlobal_d(MATFile *ph, const char *name, const mxArray *mp, const char * file, int linenum);


#if defined(MATLAB_MEX_FILE)


extern int mexAtExit_d(void (*exit_fcn)(void), const char * file, int linenum);


extern int mexCallMATLAB_d(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[], 
                    const char *fcn_name, const char * file, int linenum);


extern void mexErrMsgTxt_d(const char * error_msg, const char * file, int linenum);


extern int mexEvalString_d(const char *str, const char * file, int linenum);


extern const mxArray *mexGet_d(double handle, const char *property, const char * file, int linenum);


extern mxArray *mexGetVariable_d(const char *workspace, const char *name, const char * file, int linenum);


extern const mxArray *mexGetVariablePtr_d(const char *workspace, const char *name, const char * file, int linenum);


extern bool mexIsGlobal_d(const mxArray *pa, const char * file, int linenum);


extern void mexMakeArrayPersistent_d(mxArray *pa, const char * file, int linenum);


extern void mexMakeMemoryPersistent_d(void *ptr, const char * file, int linenum);

extern  int status;


extern int mexSet_d(double handle, const char *property, mxArray *value, const char * file, int linenum);


extern void mexSetTrapFlag_d(int value, const char * file, int linenum);


extern void mexWarnMsgTxt_d(const char *error_msg, const char * file, int linenum);


#endif /* MATLAB_MEX_FILE */


extern void * mxMalloc_d(size_t n, const char * file, int linenum);


extern bool mxIsOpaque_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsObject_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsInt64_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsUint64_d(const mxArray *pa, const char * file, int linenum);


extern mxChar *mxGetChars_d(const mxArray *pa, const char * file, int linenum);


extern void mxSetFromGlobalWS_d(mxArray *pa, bool global, const char * file, int linenum);


extern mxArray *mxCreateNumericMatrix_d(int m, int n, mxClassID classid, int cmplx_flag, const char * file, int linenum);


extern mxLogical *mxGetLogicals_d(const mxArray *pa, const char * file, int linenum);


extern mxArray *mxCreateLogicalArray_d(int ndim, const int *dims, const char * file, int linenum);


extern mxArray *mxCreateLogicalMatrix_d(unsigned int m, unsigned int n, const char * file, int linenum);


extern mxArray *mxCreateLogicalScalar_d(bool value, const char * file, int linenum);


extern mxArray *mxCreateDoubleScalar_d(double value, const char * file, int linenum);


extern mxArray *mxCreateSparseLogicalMatrix_d(int m, int n, int nzmax, const char * file, int linenum);


extern char *mxArrayToString_d(const mxArray *pa, const char * file, int linenum);


extern int mxAddField_d(mxArray *pa, const char *fieldname, const char * file, int linenum);


extern void mxRemoveField_d(mxArray *pa, int field, const char * file, int linenum);


extern bool mxIsFinite_d(double x, const char * file, int linenum);


extern bool mxIsInf_d(double x, const char * file, int linenum);


extern bool mxIsNaN_d(double x, const char * file, int linenum);


extern int mxCalcSingleSubscript_d(const mxArray *pa, int nsubs, const int *subs,
                            const char * file, int linenum);


extern void * mxCalloc_d(size_t nmemb, size_t size, const char * file, int linenum);


extern mxArray *mxCreateCellArray_d(int ndim, const int *dims, const char * file, int linenum);


extern mxArray *mxCreateCellMatrix_d(int m, int n, const char * file, int linenum);


extern mxArray *mxCreateCharArray_d(int ndim, const int *dims, const char * file, int linenum);


extern mxArray *mxCreateCharMatrixFromStrings_d(int m, const char **str, const char * file, int linenum);


extern mxArray *mxCreateNumericArray_d(int ndim, const int *dims, mxClassID classid, 
                                mxComplexity flag, const char * file, int linenum);


extern mxArray *mxCreateDoubleMatrix_d(int m, int n, mxComplexity flag, const char * file, int linenum);


extern mxArray *mxCreateSparse_d(int m, int n, int nzmax, mxComplexity flag, const char * file, int linenum);


extern mxArray *mxCreateString_d(const char *str, const char * file, int linenum);


extern mxArray *mxCreateStructArray_d(int ndim, const int *dims, int nfields,
                               const char **fieldnames, const char * file, int linenum);


extern mxArray *mxCreateStructMatrix_d(int m, int n, int nfields, 
								  const char **fieldnames, const char * file, int linenum);


extern void mxDestroyArray_d(mxArray *pa, const char * file, int linenum);


extern mxArray *mxDuplicateArray_d(const mxArray *pa,
                            const char *file,
                            const int linenum);


extern void mxFree_d(void *pm, const char * file, int linenum);


extern mxArray *mxGetCell_d(const mxArray *pa, int i, const char * file, int linenum);


extern mxClassID mxGetClassID_d(const mxArray *pa, const char * file, int linenum);


extern const char *mxGetClassName_d(const mxArray *pa, const char * file, int linenum);


extern void *mxGetData_d(
    const mxArray *pa, const char * file, int linenum		
    );


extern const int *mxGetDimensions_d(const mxArray *pa, const char * file, int linenum);


extern int mxGetElementSize_d(const mxArray *pa, const char * file, int linenum);


extern mxArray *mxGetField_d(const mxArray *pa, int i, const char *fieldname, 
					const char * file, int linenum);


extern mxArray *mxGetFieldByNumber_d(const mxArray *pa, int i, int fieldnum, 
							const char * file, int linenum);


extern const char *mxGetFieldNameByNumber_d(const mxArray *pa, int n, 
									const char * file, int linenum);


extern int mxGetFieldNumber_d(const mxArray *pa, const char *name, 
						const char * file, int linenum);


extern void *mxGetImagData_d(const mxArray *pa, const char * file, int linenum);


extern int *mxGetIr_d(const mxArray *pa, const char * file, int linenum);


extern int *mxGetJc_d(const mxArray *pa, const char * file, int linenum);


extern int mxGetNumberOfDimensions_d(const mxArray *pa, const char * file, int linenum);


extern int mxGetNumberOfElements_d(const mxArray *pa, const char * file, int linenum);


extern int mxGetNumberOfFields_d(const mxArray *pa, const char * file, int linenum);


extern int mxGetNzmax_d(const mxArray *pa, const char * file, int linenum);


extern int mxGetM_d(const mxArray *pa, const char * file, int linenum);


extern int mxGetN_d(const mxArray *pa, const char * file, int linenum);


extern double *mxGetPi_d(const mxArray *pa, const char * file, int linenum);


extern double *mxGetPr_d(const mxArray *pa, const char * file, int linenum);


extern double mxGetScalar_d(const mxArray *pa, const char * file, int linenum);


extern int mxGetString_d(const mxArray *pa, char *buf, int buflen, const char * file, int linenum);


extern bool mxIsCell_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsChar_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsClass_d(const mxArray *pa, const char *name, const char * file, int linenum);


extern bool mxIsComplex_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsDouble_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsEmpty_d(
    const mxArray *pa, const char * file, int linenum		
    );


extern bool mxIsInt8_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsInt16_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsInt32_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsLogical_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsNumeric_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsSingle_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsSparse_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsStruct_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsUint8_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsUint16_d(const mxArray *pa, const char * file, int linenum);


extern bool mxIsUint32_d(const mxArray *pa, const char * file, int linenum);


extern void * mxRealloc_d(void * pm, size_t size, const char * file, int linenum);


extern void mxSetCell_d(mxArray *pa, int i, mxArray *value, const char * file, int linenum);


extern int mxSetClassName_d(mxArray *pa, const char *classname, const char * file, int linenum);


extern void mxSetData_d(mxArray *pa, void *Data, const char * file, int linenum);


extern int mxSetDimensions_d(mxArray *pa, const int *size, int ndims, const char * file, int linenum);


extern void mxSetField_d(mxArray *pa, int i, const char *fieldname, mxArray *value,
				const char * file, int linenum);


extern void mxSetFieldByNumber_d(mxArray *pa, int i, int fieldnum, mxArray *value, 
						const char * file, int linenum);


extern void mxSetImagData_d(mxArray *pa, void *ImagData, const char * file, int linenum);


extern void mxSetIr_d(mxArray *pa, int *ir, const char * file, int linenum);


extern void mxSetJc_d(mxArray *pa,int *jc, const char * file, int linenum);


extern void mxSetM_d(mxArray *pa,int m, const char * file, int linenum);

 
extern void mxSetN_d(mxArray *pa, int n, const char * file, int linenum);


extern void mxSetNzmax_d(mxArray *pa, int nzmax, const char * file, int linenum);


extern void mxSetPi_d(mxArray *pa, double *pi, const char * file, int linenum);


extern void mxSetPr_d(mxArray *pa, double *pr, const char * file, int linenum);

#ifdef __cplusplus
    }	/* extern "C" */
#endif

#endif /* mwdebug_h */
