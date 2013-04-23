
#ifndef __LIB_INF_H
#define __LIB_INF_H
//#include "mtypes.h"
#include "lang.h"
#include <WinDef.h>

///////////////////////////////////

// 操作系统类别：

#define __OS_WIN        0x80000000
#define __OS_LINUX      0x40000000
#define __OS_UNIX       0x20000000

#define OS_ALL     (__OS_WIN | __OS_LINUX | __OS_UNIX)

/*

关于跨操作系统编程支持技术说明：

  1、各支持库需要说明其具有哪些操作系统版本，及支持库中所有命令、数据类型、
     数据类型方法、数据类型事件、数据类型属性所支持的操作系统。以上这些信息，
     在该支持库所具有的所有操作系统版本中必须一致。

  2、为了和以前的支持库相兼容，所有m_nRqSysMajorVer.m_nRqSysMinorVer版本号
     小于3.6的都默认为支持Windows操作系统，包括内部的所有命令、数据类型、
     数据类型方法、数据类型事件、数据类型属性。

  3、所有组件固定属性和固定事件都支持所有操作系统。

  4、对于纯工具库不进行操作系统支持检查。

*/

typedef	SHORT		DTBOOL;		// SDT_BOOL类型的值类型
typedef	DTBOOL*		PDTBOOL;
#define	BL_TRUE		-1			// SDT_BOOL类型的真值
#define	BL_FALSE	0			// SDT_BOOL类型的假值
#define DATE double
typedef DATE*	PDATE;

///////////////////////////////////

// 以下是由系统定义的基本数据类型

#define		_SDT_NULL		0           // 空数据（内部使用，必须为零）
/*  仅用于库命令定义其参数或返回值的数据类型。
    1、当用于定义库命令参数时，_SDT_ALL可以匹配所有数据类型（数组类型必须符合要求）。
    2、由于定义为返回_SDT_ALL数据类型的库命令不允许返回数组或复合数据类型的数据
    (即用户或库自定义数据类型但不包含窗口或菜单组件)，所以_SDT_ALL类型的数据只
    可能为非数组的系统数据类型或窗口组件、菜单数据类型。*/
#define		_SDT_ALL		MAKELONG (MAKEWORD (0, 0), 0x8000)	// （内部使用）
// 数值，匹配所有类型数字，仅用于库命令定义其参数及返回值的数据类型。
//#define		_SDT_NUM		MAKELONG (MAKEWORD (1, 0), 0x8000)	// （内部使用），3.0版本中已经废弃。
	#define		SDT_BYTE		MAKELONG (MAKEWORD (1, 1), 0x8000)		// 字节
	#define		SDT_SHORT		MAKELONG (MAKEWORD (1, 2), 0x8000)		// 短整数
	#define		SDT_INT			MAKELONG (MAKEWORD (1, 3), 0x8000)		// 整数
	#define		SDT_INT64		MAKELONG (MAKEWORD (1, 4), 0x8000)		// 长整数
	#define		SDT_FLOAT		MAKELONG (MAKEWORD (1, 5), 0x8000)		// 小数
	#define		SDT_DOUBLE		MAKELONG (MAKEWORD (1, 6), 0x8000)		// 双精度小数
#define		SDT_BOOL		MAKELONG (MAKEWORD (2, 0),	0x8000)		// 逻辑
#define		SDT_DATE_TIME	MAKELONG (MAKEWORD (3, 0),	0x8000)		// 日期时间
#define		SDT_TEXT		MAKELONG (MAKEWORD (4, 0),	0x8000)		// 文本
#define		SDT_BIN			MAKELONG (MAKEWORD (5, 0),	0x8000)		// 字节集
#define		SDT_SUB_PTR		MAKELONG (MAKEWORD (6, 0),	0x8000)		// 子程序指针
//#define		_SDT_VAR_REF	MAKELONG (MAKEWORD (7, 0),	0x8000)		// 参考，3.0版本中已经废弃。
#define		SDT_STATMENT	MAKELONG (MAKEWORD (8, 0),	0x8000)
	// 子语句型，仅用于库命令定义其参数的数据类型。其数据长度为两个INT，
	// 第一个记录存根子程序地址，第二个记录该子语句所在子程序的变量栈首。
    // !!! 注意在用作库命令参数时，编译器允许其可以接收所有基本数据类型，所以必须首先判断处理
    /* 调用例子：
        if (pArgInf->m_dtDataType == SDT_BOOL)
            return pArgInf->m_bool;

        if (pArgInf->m_dtDataType == SDT_STATMENT)
        {
            DWORD dwEBP = pArgInf->m_statment.m_dwSubEBP;
            DWORD dwSubAdr = pArgInf->m_statment.m_dwStatmentSubCodeAdr;
            DWORD dwECX, dwEAX;

            _asm
            {
                mov eax, dwEBP
                call dwSubAdr
                mov dwECX, ecx
                mov dwEAX, eax
            }

            if (dwECX == SDT_BOOL)
                return dwEAX != 0;

            // 释放文本或字节集数据所分配的内存。
            if (dwECX == SDT_TEXT || dwECX == SDT_BIN)
                MFree ((void*)dwEAX);
        }

        GReportError ("用作进行条件判断的子语句参数只能接受逻辑型数据");
    */

// 用作区分系统类型、用户自定义类型、库定义数据类型
#define	DTM_SYS_DATA_TYPE_MASK		0x80000000
#define	DTM_USER_DATA_TYPE_MASK		0x40000000
#define	DTM_LIB_DATA_TYPE_MASK		0x00000000

// 用作细分用户自定义数据类型
#define	UDTM_USER_DECLARE_MASK		0x00000000	// 用户自定义复合数据类型
#define	UDTM_WIN_DECLARE_MASK		0x10000000	// 用户自定义窗口类型

// 在数据类型中的数组标志，如果某数据类型值此位置1，则表示为此数据类型的数组。
// 本标志仅用作在运行时为具有AS_RECEIVE_VAR_OR_ARRAY或AS_RECEIVE_ALL_TYPE_DATA
// 标志的库命令参数说明其为是否为数组数据，其他场合均未使用。因此其他地方均
// 可以忽略本标志。
#define	DT_IS_ARY					0x20000000
// 在数据类型中的传址标志，如果某数据类型值此位置1，则表示为此数据类型的变量地址。
// 本标志仅用作在运行时为具有AS_RECEIVE_VAR_OR_OTHER标志的库命令参数说明其为是否为
// 变量地址，其他场合均未使用。因此其他地方均可以忽略本标志。
// 本标志与上标志不能共存。
#define	DT_IS_VAR					0x20000000

typedef DWORD DATA_TYPE;
typedef DATA_TYPE* PDATA_TYPE;

// NES_ 宏为仅被易编辑环境处理的通知。
#define NES_GET_MAIN_HWND			        1
	// 取易编辑环境主窗口的句柄，用作支持支持库的AddIn。
#define	NES_RUN_FUNC				        2
	// 通知易编辑环境运行指定的功能，返回一个BOOL值。
	// dwParam1为功能号。
	// dwParam2为一个双DWORD数组指针,分别提供功能参数1和2。
#define NES_PICK_IMAGE_INDEX_DLG            7
    // 通知易编辑环境显示一个对话框列出指定图片组内的所有图片，并返回用户所选择图片的索引号。
    // dwParam1为所欲浏览的有效的图片组句柄。
    //   dwParam2如果不为NULL，则系统认为此为一个编辑框HWND窗口句柄，当用户做出有效选择后，
    // 系统将自动更改此编辑框的内容并将焦点转移上去。
    // 返回用户所选择图片的索引号(-1表示用户选择无图片)，如果用户未选择，则返回-2。

// NAS_ 宏为既被易编辑环境又被易运行环境处理的通知。
#define	NAS_GET_APP_ICON			        1000
	// 通知系统创建并返回程序的图标。
	// dwParam1为PAPP_ICON指针。
#define NAS_GET_LIB_DATA_TYPE_INFO          1002
    // 返回指定库定义数据类型的PLIB_DATA_TYPE_INFO定义信息指针。
    // dwParam1为欲检查的数据类型。
    // 如果该数据类型无效或者不为库定义数据类型，则返回NULL，否则返回PLIB_DATA_TYPE_INFO指针。
#define NAS_GET_HBITMAP                     1003
    // dwParam1为图片数据指针，dwParam2为图片数据尺寸。
    // 如果成功返回非NULL的HBITMAP句柄（注意使用完毕后释放），否则返回NULL。
#define NAS_GET_LANG_ID                     1004
    // 返回当前系统或运行环境所支持的语言ID，具体ID值请见lang.h
#define NAS_GET_VER                         1005
    // 返回当前系统或运行环境的版本号，LOWORD为主版本号，HIWORD为次版本号。
#define NAS_GET_PATH                        1006
    /* 返回当前开发或运行环境的某一类目录或文件名，目录名以“\”结束。
       dwParam1: 指定所需要的目录，可以为以下值：
         A、开发及运行环境下均有效的目录:
            1: 开发或运行环境系统所处的目录；
         B、开发环境下有效的目录(仅开发环境中有效):
            1001: 系统例程和支持库例程所在目录名
            1002: 系统工具所在目录
            1003: 系统帮助信息所在目录
            1004: 保存所有登记到系统中易模块的目录
            1005: 支持库所在的目录
            1006: 安装工具所在目录
         C、运行环境下有效的目录(仅运行环境中有效):
            2001: 用户EXE文件所处目录；
            2002: 用户EXE文件名；
       dwParam2: 接收缓冲区地址，尺寸必须为MAX_PATH。
    */
#define NAS_CREATE_CWND_OBJECT_FROM_HWND    1007
    // 通过指定HWND句柄创建一个CWND对象，返回其指针，记住此指针必须通过调用NRS_DELETE_CWND_OBJECT来释放
    // dwParam1为HWND句柄
    // 成功返回CWnd*指针，失败返回NULL
#define NAS_DELETE_CWND_OBJECT              1008
    // 删除通过NRS_CREATE_CWND_OBJECT_FROM_HWND创建的CWND对象
    // dwParam1为欲删除的CWnd对象指针
#define NAS_DETACH_CWND_OBJECT              1009
    // 取消通过NRS_CREATE_CWND_OBJECT_FROM_HWND创建的CWND对象与其中HWND的绑定
    // dwParam1为CWnd对象指针
    // 成功返回HWND,失败返回0
#define NAS_GET_HWND_OF_CWND_OBJECT         1010
    // 获取通过NRS_CREATE_CWND_OBJECT_FROM_HWND创建的CWND对象中的HWND
    // dwParam1为CWnd对象指针
    // 成功返回HWND,失败返回0
#define NAS_ATTACH_CWND_OBJECT              1011
    // 将指定HWND与通过NRS_CREATE_CWND_OBJECT_FROM_HWND创建的CWND对象绑定起来
    // dwParam1为HWND
    // dwParam2为CWnd对象指针
    // 成功返回1,失败返回0
#define	NAS_IS_EWIN							1014
    // 如果指定窗口为易语言窗口或易语言组件，返回真，否则返回假。
    // dwParam1为欲测试的HWND.

// NRS_ 宏为仅能被易运行环境处理的通知。
#define NRS_UNIT_DESTROIED			        2000
	// 通知系统指定的组件已经被销毁。
	// dwParam1为dwFormID
	// dwParam2为dwUnitID
#define NRS_CONVERT_NUM_TO_INT              2001
	// 转换其它数值格式到整数。
	// dwParam1为 PMDATA_INF 指针，其 m_dtDataType 必须为数值型。
    // 返回转换后的整数值。
#define NRS_GET_CMD_LINE_STR			    2002
	// 取当前命令行文本
	// 返回命令行文本指针，有可能为空串。
#define NRS_GET_EXE_PATH_STR                2003
	// 取当前执行文件所处目录名称
	// 返回当前执行文件所处目录文本指针。
#define NRS_GET_EXE_NAME				    2004
	// 取当前执行文件名称
	// 返回当前执行文件名称文本指针。
#define NRS_GET_UNIT_PTR				    2006
	// 取组件对象指针
	// dwParam1为WinForm的ID
	// dwParam2为WinUnit的ID
	// 成功返回有效的组件对象CWnd*指针，失败返回0。
#define NRS_GET_AND_CHECK_UNIT_PTR			2007
	// 取组件对象指针
	// dwParam1为WinForm的ID
	// dwParam2为WinUnit的ID
	// 成功返回有效的组件对象CWnd*指针，失败报告运行时错误并退出程序。
#define NRS_EVENT_NOTIFY				    2008
	// 以第一类方式通知系统产生了事件。
	// dwParam1为PEVENT_NOTIFY指针。
	//   如果返回 0 ，表示此事件已被系统抛弃，否则表示系统已经成功传递此事件到用户
	// 事件处理子程序。
#define	NRS_DO_EVENTS			            2018
	// 通知系统发送所有待处理事件。
#define NRS_GET_UNIT_DATA_TYPE              2022
	// dwParam1为WinForm的ID
	// dwParam2为WinUnit的ID
	// 成功返回有效的 DATA_TYPE ，失败返回 0 。
#define NRS_FREE_ARY                        2023
    // 释放指定数组数据。
    // dwParam1为该数据的DATA_TYPE，只能为系统数据类型。
    // dwParam2为指向该数组数据的指针。
#define NRS_MALLOC                          2024
    // 分配指定空间的内存，所有与易程序交互的内存都必须使用本通知分配。
    //   dwParam1为欲需求内存字节数。
    //   dwParam2如为0，则如果分配失败就自动报告运行时错并退出程序。
    // 如不为0，则如果分配失败就返回NULL。
    //   返回所分配内存的首地址。
#define NRS_MFREE                           2025
    // 释放已分配的指定内存。
    // dwParam1为欲释放内存的首地址。
#define NRS_MREALLOC                        2026
    // 重新分配内存。
    //   dwParam1为欲重新分配内存尺寸的首地址。
    //   dwParam2为欲重新分配的内存字节数。
    // 返回所重新分配内存的首地址，失败自动报告运行时错并退出程序。
#define	NRS_RUNTIME_ERR			            2027
	// 通知系统已经产生运行时错误。
	// dwParam1为char*指针，说明错误文本。
#define	NRS_EXIT_PROGRAM                    2028
	// 通知系统退出用户程序。
	// dwParam1为退出代码，该代码将被返回到操作系统。
#define NRS_GET_PRG_TYPE                    2030
    // 返回当前用户程序的类型，为PT_DEBUG_RUN_VER（调试版）或PT_RELEASE_RUN_VER（发布版）。
#define NRS_EVENT_NOTIFY2				    2031
	// 以第二类方式通知系统产生了事件。
	// dwParam1为PEVENT_NOTIFY2指针。
	//   如果返回 0 ，表示此事件已被系统抛弃，否则表示系统已经成功传递此事件到用户
	// 事件处理子程序。
#define NRS_GET_WINFORM_COUNT               2032
    // 返回当前程序的窗体数目。
#define NRS_GET_WINFORM_HWND                2033
    // 返回指定窗体的窗口句柄，如果该窗体尚未被载入，返回NULL。
	// dwParam1为窗体索引。
#define NRS_GET_BITMAP_DATA                 2034
    // 返回指定HBITMAP的图片数据，成功返回包含BMP图片数据的HGLOBAL句柄，失败返回NULL。
	// dwParam1为欲获取其图片数据的HBITMAP。
#define NRS_FREE_COMOBJECT                  2035
    // 通知系统释放指定的DTP_COM_OBJECT类型COM对象。
    // dwParam1为该COM对象的地址指针。
#define NRS_CHK_TAB_VISIBLE                 2039
	// 当选择夹子夹被切换后, 使用本消息通知易系统


/*///////////////////////////////////////////////////////////////////*/
// 系统可以通知库的码值：
#define	NL_SYS_NOTIFY_FUNCTION		    1
	//   告知库通知系统用的函数指针，在装载支持库前通知，可能有多次，
	// 后通知的值应该覆盖前面所通知的值），忽略返回值。
	//   库可将此函数指针记录下来以便在需要时使用它通知信息到系统。
	//   dwParam1: (PFN_NOTIFY_SYS)
#define NL_FREE_LIB_DATA                6
    // 通知支持库释放资源准备退出及释放指定的附加数据。


/*///////////////////////////////////////////////////////////////////*/

#define NR_OK		0
#define NR_ERR		-1

    typedef INT (WINAPI *PFN_NOTIFY_LIB) (INT nMsg, DWORD dwParam1, DWORD dwParam2);
	    // 此函数用作系统通知库有关事件。
    typedef INT (WINAPI *PFN_NOTIFY_SYS) (INT nMsg, DWORD dwParam1, DWORD dwParam2);
	    // 此函数用作库通知系统有关事件。
////////////////////////////

#define	WU_GET_WND_PTR			(WM_APP + 2)	// 用作在非核心支持库中的窗口组件支持事件反馈。

#define WU_INIT                 (WM_APP + 111)  //窗口创建完毕后给窗口中每一个易语言组件窗口发送此消息

// 窗口创建完毕后,此信息被发送给所有组件以让其进行需要其他组件数据配合的初始化工作.
// 本消息与WU_INIT的区别为，在本消息处理过程中所有触发的windows消息均会得到处理，
// 而在WU_INIT处理过程中所有触发的windows消息均会被屏蔽。
#define	WU_INIT2                (WM_APP + 118) 

typedef double DOUBLE;

#ifndef __GCC_
#pragma pack (push, old_value)   // 保存VC++编译器结构对齐字节数。
#pragma pack (1)    // 设置为以一字节对齐。
#endif

	typedef struct
	{
		union
		{
			BYTE	      m_byte;         // SDT_BYTE
			SHORT	      m_short;        // SDT_SHORT
			INT		      m_int;          // SDT_INT
			DWORD	      m_uint;         // (DWORD)SDT_INT
			INT64	      m_int64;        // SDT_INT64
			FLOAT	      m_float;        // SDT_FLOAT
			DOUBLE	      m_double;       // SDT_DOUBLE
			DATE          m_date;         // SDT_DATE_TIME
			BOOL          m_bool;         // SDT_BOOL
			char*         m_pText;        // SDT_TEXT，不可能为NULL。
			// !!!为了避免修改到常量段(m_pText有可能指向常量段区域)中的数据，
			// 只可读取而不可更改其中的内容，下同。
			LPBYTE        m_pBin;         // SDT_BIN，不可能为NULL，!!!只可读取而不可更改其中的内容。
			DWORD         m_dwSubCodeAdr; // SDT_SUB_PTR，记录子程序代码地址。
			void*         m_pCompoundData;// 复合数据类型数据指针，指针所指向数据的格式请见 run.h 。
			// 可以直接更改其中的数据成员，但是如果需要必须首先释放该成员。
			void*         m_pAryData;     // 数组数据指针，指针所指向数据的格式请见 run.h 。
			// 注意如果为文本或字节集数组，则成员数据指针可能为NULL。
			// !!! 只可读取而不可更改其中的内容。

			// 为指向变量地址的指针，仅当传入参数到库命令实现函数时才有用。
			BYTE*	m_pByte;         // SDT_BYTE*
			SHORT*	m_pShort;        // SDT_SHORT*
			INT*	m_pInt;          // SDT_INT*
			DWORD*	m_pUInt;         // ((DWORD)SDT_INT)*
			INT64*	m_pInt64;        // SDT_INT64*
			FLOAT*	m_pFloat;        // SDT_FLOAT*
			DOUBLE*	m_pDouble;       // SDT_DOUBLE*
			DATE*   m_pDate;         // SDT_DATE_TIME*
			BOOL*   m_pBool;         // SDT_BOOL*
			char**  m_ppText;        // SDT_TEXT，*m_ppText可能为NULL。
			// 注意写入新值之前必须释放前值，即：MFree (*m_ppText)。
			// !!!不可直接更改*m_ppText所指向的内容，只能释放原指针后换入新指针。
			LPBYTE* m_ppBin;         // SDT_BIN，*m_ppBin可能为NULL。
			// 注意写入新值之前必须释放前值，即：MFree (*m_ppBin)。
			// !!!不可直接更改*m_ppBin所指向的内容，只能释放原指针后换入新指针。
			DWORD*  m_pdwSubCodeAdr; // SDT_SUB_PTR，子程序代码地址变量。
			void**  m_ppCompoundData;   // 复合数据类型变量。
			// 可以直接更改其中的数据成员，但是如果需要必须首先释放该成员。
			void**  m_ppAryData;        // 数组数据变量，注意：
			// 1、写入新值之前必须释放原值（使用NRS_FREE_VAR通知）。
			// 2、变量如果为文本或字节集数组，则成员数据指针可能为NULL。
			// !!!不可直接更改*m_ppAryData所指向的内容，只能释放原指针后换入新指针。
		};

		// 1、当用作传入参数时，如果该参数具有 AS_RECEIVE_VAR_OR_ARRAY 或
		//    AS_RECEIVE_ALL_TYPE_DATA 标志，且为数组数据，则包含数组标志 DT_IS_ARY ，
		//    这也是 DT_IS_ARY 标志的唯一使用场合。
		// 2、当用作传递参数数据时，如果为空白数据，则为 _SDT_NULL 。
		DATA_TYPE m_dtDataType;
	} MDATA_INF;
	typedef MDATA_INF* PMDATA_INF;

#ifndef __GCC_
#pragma pack (pop, old_value)    // 恢复VC++编译器结构对齐字节数。
#endif


#endif

