#pragma once 

//#######################################################################################
// D E F I N E S
//#######################################################################################
/*
#if DBG
#define dprintf DbgPrint
#else
#define dprintf
#endif
*/
#define dprintf DbgPrint
//
// Device IO Control Codes
//
#define IOCTL_BASE          0x800
#define MY_CTL_CODE(i)        \
    CTL_CODE                  \
    (                         \
        FILE_DEVICE_UNKNOWN,  \
        IOCTL_BASE + i,       \
        METHOD_BUFFERED,      \
        FILE_ANY_ACCESS       \
    )

#define IOCTL_HELLO_WORLD            MY_CTL_CODE(0)
