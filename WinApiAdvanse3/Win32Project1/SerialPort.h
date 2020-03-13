//
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE. IT CAN BE DISTRIBUTED FREE OF CHARGE AS LONG AS THIS HEADER 
//  REMAINS UNCHANGED.
//
//  Email:  yetiicb@hotmail.com
//
//  Copyright (C) 2002-2003 Idael Cardoso. 
//


#if !defined(AFX_SERIALPORT_H__731AC17A_665D_4C64_AAA7_6D284B3B7AE8__INCLUDED_)
#define AFX_SERIALPORT_H__731AC17A_665D_4C64_AAA7_6D284B3B7AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSerialPort  
{
public:
	void Set_RTS_State(BOOL state);
	void Set_DTR_State(BOOL state);
	BOOL Get_RI_State();
	BOOL Get_DSR_State();
	BOOL Get_CTS_State();
	BOOL Get_CD_State();
	virtual DWORD Write(LPVOID Buffer, DWORD BufferSize);
	virtual DWORD Read(LPVOID Buffer, DWORD BufferSize);
	virtual BOOL IsOpen();
	virtual void Close();
    // Use PortName usually "COM1:" ... "COM4:" note that the name must end by ":"
	virtual BOOL Open(LPCTSTR PortName, DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits, DWORD DesiredAccess = GENERIC_READ|GENERIC_WRITE);
	CSerialPort();
	virtual ~CSerialPort();

protected:
	HANDLE m_PortHandle;
};

#endif // !defined(AFX_SERIALPORT_H__731AC17A_665D_4C64_AAA7_6D284B3B7AE8__INCLUDED_)
