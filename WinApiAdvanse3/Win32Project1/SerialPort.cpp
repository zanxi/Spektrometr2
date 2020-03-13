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

#include ".\stdwin.h"
//#include "Test.h"
#include "SerialPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialPort::CSerialPort()
  : m_PortHandle(INVALID_HANDLE_VALUE)
  {
   
  }

CSerialPort::~CSerialPort()
  {
    Close();
  }

BOOL CSerialPort::Open(LPCTSTR PortName, DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits, DWORD DesiredAccess)
  {
    Close();
    m_PortHandle = CreateFile(PortName, DesiredAccess, 0, NULL, OPEN_EXISTING, 0, 0);
    if (m_PortHandle != INVALID_HANDLE_VALUE)
      {
        DCB dcb;
        CString s;
        dcb.DCBlength = sizeof(dcb);
        GetCommState(m_PortHandle, &dcb);
        dcb.BaudRate = BaudRate;
        dcb.ByteSize = ByteSize;
        dcb.StopBits = StopBits;
        dcb.Parity = Parity;
        dcb.fDsrSensitivity = 0;
        dcb.fOutxCtsFlow = 0;
        dcb.fOutxDsrFlow = 0;
        dcb.fInX = 0;
        dcb.fOutX = 0;
        dcb.fDtrControl = DTR_CONTROL_DISABLE; //DTR and RTS 0
        dcb.fRtsControl = RTS_CONTROL_DISABLE; 
    
        SetCommState(m_PortHandle, &dcb);
    
        COMMTIMEOUTS touts;
        touts.ReadIntervalTimeout = 0;
        touts.ReadTotalTimeoutMultiplier = 0;
        touts.ReadTotalTimeoutConstant = 10;
        touts.WriteTotalTimeoutConstant = 10;
        touts.WriteTotalTimeoutMultiplier = 0;
        SetCommTimeouts(m_PortHandle, &touts);
        SetCommMask (m_PortHandle, EV_CTS | EV_DSR | EV_RING | EV_RLSD);
        PurgeComm(m_PortHandle, PURGE_TXCLEAR | PURGE_RXCLEAR);
        return TRUE;
      }
    else
      {
		return FALSE; // Use GetLastError() to know the reason
      }
  }

void CSerialPort::Close()
  {
    if (m_PortHandle != INVALID_HANDLE_VALUE)
      {
        CloseHandle(m_PortHandle);
        m_PortHandle = INVALID_HANDLE_VALUE;
      }
  }

BOOL CSerialPort::IsOpen()
  {
    return (m_PortHandle != INVALID_HANDLE_VALUE);
  }

DWORD CSerialPort::Read(LPVOID Buffer, DWORD BufferSize)
  {
    DWORD Res(0);
    if (m_PortHandle != INVALID_HANDLE_VALUE)
      {
        ReadFile(m_PortHandle, Buffer, BufferSize, &Res, NULL);
      }
    return Res;  
  }

DWORD CSerialPort::Write(LPVOID Buffer, DWORD BufferSize)
  {
    DWORD Res(0);
    if (m_PortHandle != INVALID_HANDLE_VALUE)
      {
        WriteFile(m_PortHandle, Buffer, BufferSize, &Res, NULL);
      }
    return Res;
  }

BOOL CSerialPort::Get_CD_State()
  {
    if (m_PortHandle != INVALID_HANDLE_VALUE)
      {
        DWORD ModemStat;
        if (GetCommModemStatus(m_PortHandle, &ModemStat))
          {
            return (ModemStat & MS_RLSD_ON) > 0; //Not sure
          }
        else 
          {
            return FALSE;
          }
      }
    else 
      {
        return FALSE;
      }
  }

BOOL CSerialPort::Get_CTS_State()
  {
    if (m_PortHandle != INVALID_HANDLE_VALUE)
      {
        DWORD ModemStat;
        if (GetCommModemStatus(m_PortHandle, &ModemStat))
          {
            return (ModemStat & MS_CTS_ON) > 0;
          }
        else 
          {
            return FALSE;
          }
      }
    else 
      {
        return FALSE;
      }
  }

BOOL CSerialPort::Get_DSR_State()
  { 
    if (m_PortHandle != INVALID_HANDLE_VALUE)
      {
        DWORD ModemStat;
        if (GetCommModemStatus(m_PortHandle, &ModemStat))
          {
            return (ModemStat & MS_DSR_ON) > 0;
          }
        else 
          {
            return FALSE;
          }
      }
    else 
      {
        return FALSE;
      }
  }

BOOL CSerialPort::Get_RI_State()
  { 
    if (m_PortHandle != INVALID_HANDLE_VALUE)
      {
        DWORD ModemStat;
        if (GetCommModemStatus(m_PortHandle, &ModemStat))
          {
            return (ModemStat & MS_RING_ON) > 0;
          }
        else 
          {
            return FALSE;
          }
      }
    else 
      {
        return FALSE;
      }
  }

void CSerialPort::Set_DTR_State(BOOL state)
  { 
    if (m_PortHandle != INVALID_HANDLE_VALUE)
      {
        EscapeCommFunction(m_PortHandle, (state?SETDTR:CLRDTR));
      }
  }

void CSerialPort::Set_RTS_State(BOOL state)
  { 
    if (m_PortHandle != INVALID_HANDLE_VALUE)
      {
        EscapeCommFunction(m_PortHandle, (state?SETRTS:CLRRTS));
      }
  }
