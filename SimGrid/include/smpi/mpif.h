! -*- fortran -*-
! Copyright (c) 2010, 2012-2014. The SimGrid Team.
! All rights reserved.

! This program is free software; you can redistribute it and/or modify it
! under the terms of the license (GNU LGPL) which comes with this package.

! SMPI's Fortran 77 include file

      integer MPI_THREAD_SINGLE, MPI_THREAD_FUNNELED
      integer MPI_THREAD_SERIALIZED, MPI_THREAD_MULTIPLE
      parameter(MPI_THREAD_SINGLE=0)
      parameter(MPI_THREAD_FUNNELED=1)
      parameter(MPI_THREAD_SERIALIZED=2)
      parameter(MPI_THREAD_MULTIPLE=3)

      integer MPI_MAX_PROCESSOR_NAME, MPI_MAX_ERROR_STRING
      integer MPI_MAX_DATAREP_STRIN, MPI_MAX_INFO_KEY
      integer MPI_MAX_INFO_VAL, MPI_MAX_OBJECT_NAME, MPI_MAX_PORT_NAME
      integer MPI_ANY_SOURCE, MPI_PROC_NULL, MPI_ANY_TAG, MPI_UNDEFINED
      integer MPI_SOURCE, MPI_TAG, MPI_ERROR
      integer MPI_VERSION, MPI_SUBVERSION
      integer MPI_LOCK_EXCLUSIVE, MPI_LOCK_SHARED
      parameter(MPI_MAX_PROCESSOR_NAME=100)
      parameter(MPI_MAX_ERROR_STRING=100)
      parameter(MPI_MAX_DATAREP_STRIN =100)
      parameter(MPI_MAX_INFO_KEY=100)
      parameter(MPI_MAX_INFO_VAL=100)
      parameter(MPI_MAX_OBJECT_NAME=100)
      parameter(MPI_MAX_PORT_NAME=100)
      parameter(MPI_ANY_SOURCE=-555)
      parameter(MPI_PROC_NULL=-666)
      parameter(MPI_ANY_TAG=-444)
      parameter(MPI_UNDEFINED=-333)
      parameter(MPI_SOURCE=1)
      parameter(MPI_TAG=2)
      parameter(MPI_ERROR=3)
      parameter(MPI_VERSION=1)
      parameter(MPI_SUBVERSION=1)
      parameter(MPI_LOCK_EXCLUSIVE=1)
      parameter(MPI_LOCK_SHARED=2)
      integer MPI_MODE_NOSTORE, MPI_MODE_NOPUT, MPI_MODE_NOPRECEDE
      integer MPI_MODE_NOSUCCEED, MPI_MODE_NOCHECK
      parameter(MPI_MODE_NOSTORE=1)
      parameter(MPI_MODE_NOPUT=2)
      parameter(MPI_MODE_NOPRECEDE=4)
      parameter(MPI_MODE_NOSUCCEED=8)
      parameter(MPI_MODE_NOCHECK=16)

      integer MPI_SUCCESS, MPI_ERR_COMM, MPI_ERR_ARG, MPI_ERR_TYPE
      integer MPI_ERR_REQUEST, MPI_ERR_INTERN, MPI_ERR_COUNT
      integer MPI_ERR_RANK, MPI_ERR_OTHER, MPI_ERR_UNKNOWN
      integer MPI_ERR_TAG, MPI_ERR_TRUNCATE, MPI_ERR_GROUP, MPI_ERR_OP
      integer MPI_ERR_LASTCODE
      integer MPI_IDENT, MPI_SIMILAR, MPI_UNEQUAL, MPI_CONGRUENT
      parameter(MPI_SUCCESS=0)
      parameter(MPI_ERR_COMM=1)
      parameter(MPI_ERR_ARG=2)
      parameter(MPI_ERR_TYPE=3)
      parameter(MPI_ERR_REQUEST=4)
      parameter(MPI_ERR_INTERN=5)
      parameter(MPI_ERR_COUNT=6)
      parameter(MPI_ERR_RANK=7)
      parameter(MPI_ERR_TAG=8)
      parameter(MPI_ERR_TRUNCATE=9)
      parameter(MPI_ERR_GROUP=10)
      parameter(MPI_ERR_OP=11)
      parameter(MPI_ERR_OTHER=12)
      parameter(MPI_ERR_UNKNOWN=13)
      parameter(MPI_ERR_LASTCODE=74)
      parameter(MPI_IDENT=0)
      parameter(MPI_SIMILAR=1)
      parameter(MPI_UNEQUAL=2)
      parameter(MPI_CONGRUENT=3)

      integer MPI_NULL_COPY_FN, MPI_NULL_DELETE_FN
      parameter(MPI_NULL_COPY_FN =0)
      parameter(MPI_NULL_DELETE_FN =0)
      integer MPI_COMM_NULL_COPY_FN, MPI_COMM_NULL_DELETE_FN
      parameter(MPI_COMM_NULL_COPY_FN =0)
      parameter(MPI_COMM_NULL_DELETE_FN =0)
      integer MPI_COMM_NULL_DUP_FN, MPI_COMM_DUP_FN
      parameter(MPI_COMM_NULL_DUP_FN =0)
      parameter(MPI_COMM_DUP_FN =0)

      integer MPI_ROOT, MPI_INFO_NULL,MPI_COMM_TYPE_SHARED
      parameter(MPI_ROOT=0)
      parameter(MPI_INFO_NULL=0)
      parameter(MPI_COMM_TYPE_SHARED=1)

! Attribute keys
      integer MPI_IO, MPI_HOST, MPI_WTIME_IS_GLOBAL, MPI_APPNUM
      integer MPI_TAG_UB, MPI_TAG_LB
      integer MPI_UNIVERSE_SIZE, MPI_LASTUSEDCODE
      parameter(MPI_IO=-1)
      parameter(MPI_HOST=-2)
      parameter(MPI_WTIME_IS_GLOBAL=-3)
      parameter(MPI_APPNUM=-4)
      parameter(MPI_TAG_UB=-5)
      parameter(MPI_TAG_LB=-6)
      parameter(MPI_UNIVERSE_SIZE=-7)
      parameter(MPI_LASTUSEDCODE=-8)

! These should be ordered as in smpi_f77.c
      integer MPI_COMM_NULL, MPI_COMM_WORLD, MPI_COMM_SELF
      parameter(MPI_COMM_NULL=-1)
      parameter(MPI_COMM_SELF=-2)
      parameter(MPI_COMM_WORLD=0)

      integer MPI_GROUP_NULL, MPI_GROUP_EMPTY
      parameter(MPI_GROUP_NULL=-1)
      parameter(MPI_GROUP_EMPTY=-2)

      integer MPI_WIN_NULL
      parameter(MPI_WIN_NULL=-1)

      integer MPI_ERRORS_RETURN, MPI_ERRORS_ARE_FATAL
      integer MPI_ERRHANDLER_NULL
      parameter(MPI_ERRORS_RETURN=0)
      parameter(MPI_ERRORS_ARE_FATAL=1)
      parameter(MPI_ERRHANDLER_NULL=2)

! This should be equal to the number of int fields in MPI_Status
      integer MPI_STATUS_SIZE
      parameter(MPI_STATUS_SIZE=4)

! These should be ordered as in smpi_f77.c
      integer MPI_IN_PLACE, MPI_BOTTOM
      integer MPI_STATUS_IGNORE, MPI_STATUSES_IGNORE
      common /smpi/ MPI_IN_PLACE, MPI_BOTTOM
      common /smpi/ MPI_STATUS_IGNORE, MPI_STATUSES_IGNORE

      integer MPI_REQUEST_NULL
      parameter(MPI_REQUEST_NULL=-1)

      integer MPI_INTEGER_KIND
      parameter(MPI_INTEGER_KIND=4)

      integer MPI_DATATYPE_NULL, MPI_BYTE, MPI_CHARACTER, MPI_LOGICAL
      integer MPI_INTEGER, MPI_INTEGER1, MPI_INTEGER2, MPI_INTEGER4
      integer MPI_INTEGER8, MPI_REAL, MPI_REAL4, MPI_REAL8
      integer MPI_DOUBLE_PRECISION, MPI_COMPLEX, MPI_DOUBLE_COMPLEX
      integer MPI_2INTEGER, MPI_LOGICAL1, MPI_LOGICAL2, MPI_LOGICAL4
      integer MPI_LOGICAL8, MPI_2REAL, MPI_2DOUBLE_PRECISION
      integer MPI_AINT, MPI_OFFSET, MPI_COUNT
      integer MPI_REAL16, MPI_COMPLEX8,MPI_COMPLEX16,MPI_COMPLEX32
      integer MPI_PACKED
      parameter(MPI_DATATYPE_NULL=-1)
      parameter(MPI_BYTE=0)
      parameter(MPI_CHARACTER=1)
      parameter(MPI_LOGICAL=2)
      parameter(MPI_INTEGER=3)
      parameter(MPI_INTEGER1=4)
      parameter(MPI_INTEGER2=5)
      parameter(MPI_INTEGER4=6)
      parameter(MPI_INTEGER8=7)
      parameter(MPI_REAL=8)
      parameter(MPI_REAL4=9)
      parameter(MPI_REAL8=10)
      parameter(MPI_DOUBLE_PRECISION=11)
      parameter(MPI_COMPLEX=12)
      parameter(MPI_DOUBLE_COMPLEX=13)
      parameter(MPI_2INTEGER=14)
      parameter(MPI_LOGICAL1=15)
      parameter(MPI_LOGICAL2=16)
      parameter(MPI_LOGICAL4=17)
      parameter(MPI_LOGICAL8=18)
      parameter(MPI_2REAL=19)
      parameter(MPI_2DOUBLE_PRECISION=20)
      parameter(MPI_AINT=21)
      parameter(MPI_OFFSET=22)
      parameter(MPI_COUNT=23)
      parameter(MPI_REAL16=24)
      parameter(MPI_COMPLEX8=25)
      parameter(MPI_COMPLEX16=26)
      parameter(MPI_COMPLEX32=27)
      parameter(MPI_PACKED=28)

! These should be ordered as in smpi_f77.c
      integer MPI_OP_NULL,MPI_MAX, MPI_MIN, MPI_MAXLOC, MPI_MINLOC
      integer MPI_SUM, MPI_PROD, MPI_LAND, MPI_LOR, MPI_LXOR, MPI_BAND
      integer MPI_BOR, MPI_BXOR
      parameter(MPI_OP_NULL=-1)
      parameter(MPI_MAX=0)
      parameter(MPI_MIN=1)
      parameter(MPI_MAXLOC=2)
      parameter(MPI_MINLOC=3)
      parameter(MPI_SUM=4)
      parameter(MPI_PROD=5)
      parameter(MPI_LAND=6)
      parameter(MPI_LOR=7)
      parameter(MPI_LXOR=8)
      parameter(MPI_BAND=9)
      parameter(MPI_BOR=10)
      parameter(MPI_BXOR=11)

      INTEGER MPI_ADDRESS_KIND, MPI_OFFSET_KIND
      PARAMETER (MPI_ADDRESS_KIND=8)
      PARAMETER (MPI_OFFSET_KIND=8)

      integer MPI_COMBINER_NAMED, MPI_COMBINER_DUP
      integer MPI_COMBINER_CONTIGUOUS, MPI_COMBINER_VECTOR
      integer MPI_COMBINER_HVECTOR_INTEGER, MPI_COMBINER_HVECTOR
      integer MPI_COMBINER_INDEXED, MPI_COMBINER_HINDEXED_INTEGER
      integer MPI_COMBINER_HINDEXED, MPI_COMBINER_INDEXED_BLOCK
      integer MPI_COMBINER_STRUCT_INTEGER, MPI_COMBINER_STRUCT
      integer MPI_COMBINER_SUBARRAY, MPI_COMBINER_DARRAY
      integer MPI_COMBINER_F90_REAL, MPI_COMBINER_F90_COMPLEX
      integer MPI_COMBINER_F90_INTEGER, MPI_COMBINER_RESIZED
      integer MPI_COMBINER_HINDEXED_BLOCK

      parameter( MPI_COMBINER_NAMED=0)
      parameter( MPI_COMBINER_DUP=1)
      parameter( MPI_COMBINER_CONTIGUOUS=2)
      parameter( MPI_COMBINER_VECTOR=3)
      parameter( MPI_COMBINER_HVECTOR_INTEGER=4)
      parameter( MPI_COMBINER_HVECTOR=5)
      parameter( MPI_COMBINER_INDEXED=6)
      parameter( MPI_COMBINER_HINDEXED_INTEGER=7)
      parameter( MPI_COMBINER_HINDEXED=8)
      parameter( MPI_COMBINER_INDEXED_BLOCK=9)
      parameter( MPI_COMBINER_STRUCT_INTEGER=10)
      parameter( MPI_COMBINER_STRUCT=11)
      parameter( MPI_COMBINER_SUBARRAY=12)
      parameter( MPI_COMBINER_DARRAY=13)
      parameter( MPI_COMBINER_F90_REAL=14)
      parameter( MPI_COMBINER_F90_COMPLEX=15)
      parameter( MPI_COMBINER_F90_INTEGER=16)
      parameter( MPI_COMBINER_RESIZED=17)
      parameter( MPI_COMBINER_HINDEXED_BLOCK=18)

      integer MPI_ORDER_C, MPI_ORDER_FORTRAN
      parameter(MPI_ORDER_C=1)
      parameter(MPI_ORDER_FORTRAN=0)

      external MPI_INIT, MPI_FINALIZE, MPI_ABORT
      external MPI_COMM_RANK, MPI_COMM_SIZE, MPI_COMM_DUP
      external MPI_SEND_INIT, MPI_ISEND, MPI_SEND, MPI_COMM_SPLIT
      external MPI_RECV_INIT, MPI_IRECV, MPI_RECV
      external MPI_START, MPI_STARTALL
      external MPI_WAIT, MPI_WAITANY, MPI_WAITALL
      external MPI_BCAST, MPI_BARRIER, MPI_REDUCE, MPI_ALLREDUCE
      external MPI_SCATTER, MPI_GATHER, MPI_ALLGATHER, MPI_SCAN
      external MPI_ALLTOALL, MPI_GATHERV, MPI_SENDRECV

      external MPI_WTIME
      external MPI_WTICK

      double precision MPI_WTIME
      double precision MPI_WTICK

      external smpi_execute_flops
      external smpi_execute
      external smpi_get_host_power_peak_at
      external smpi_get_host_current_power_peak
      external smpi_get_host_nb_pstates
      external smpi_set_host_pstate
      external smpi_get_host_consumed_energy

      double precision smpi_get_host_power_peak_at
      double precision smpi_get_host_current_power_peak
      integer smpi_get_host_nb_pstates
      double precision smpi_get_host_consumed_energy
