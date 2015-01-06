/*=========================================================================

  Program:   Open IGT Link -- Example for Image Meta Data Client
  Module:    $RCSfile: $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>
#include <math.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "igtlOSUtil.h"
#include "igtlImageMessage.h"
#include "igtlPolyDataMessage.h"
#include "igtlClientSocket.h"


int ReceivePolyData(igtl::ClientSocket::Pointer& socket, igtl::MessageHeader::Pointer& header);


int main(int argc, char* argv[])
{
  //------------------------------------------------------------
  // Parse Arguments

  if (argc != 3) // check number of arguments
    {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <hostname> <port>"    << std::endl;
    std::cerr << "    <hostname> : IP or host name"                    << std::endl;
    std::cerr << "    <port>     : Port # (18944 in Slicer default)"   << std::endl;
    exit(0);
    }

  char*  hostname = argv[1];
  int    port     = atoi(argv[2]);

  //------------------------------------------------------------
  // Establish Connection
  igtl::ClientSocket::Pointer socket;
  socket = igtl::ClientSocket::New();
  int r = socket->ConnectToServer(hostname, port);

  if (r != 0)
    {
    std::cerr << "Cannot connect to the server." << std::endl;
    exit(0);
    }

  //------------------------------------------------------------
  // loop
  for (int i = 0; i < 10; i ++)
    {
    //------------------------------------------------------------
    // Send request data
    igtl::GetPolyDataMessage::Pointer getPolyDataMsg;
    getPolyDataMsg = igtl::GetPolyDataMessage::New();
    getPolyDataMsg->SetDeviceName("Client");
    getPolyDataMsg->Pack();
    socket->Send(getPolyDataMsg->GetPackPointer(), getPolyDataMsg->GetPackSize());
    
    //------------------------------------------------------------
    // Wait for a reply
    igtl::MessageHeader::Pointer headerMsg;
    headerMsg = igtl::MessageHeader::New();
    headerMsg->InitPack();
    int rs = socket->Receive(headerMsg->GetPackPointer(), headerMsg->GetPackSize());
    if (rs == 0)
      {
      std::cerr << "Connection closed." << std::endl;
      socket->CloseSocket();
      exit(0);
      }
    if (rs != headerMsg->GetPackSize())
      {
      std::cerr << "Message size information and actual data size don't match." << std::endl; 
      exit(0);
      }

    headerMsg->Unpack();
    if (strcmp(headerMsg->GetDeviceType(), "POLYDATA") == 0)
      {
      ReceivePolyData(socket, headerMsg);
      }
    else
      {
      std::cerr << "Invalid response from the server:" << headerMsg->GetDeviceName() << std::endl; 
      exit(0);
      }

    igtl::Sleep(500); // wait
    }
    
  //------------------------------------------------------------
  // Close connection
  socket->CloseSocket();

}


int ReceivePolyData(igtl::ClientSocket::Pointer& socket, igtl::MessageHeader::Pointer& header)
{

  std::cout << "Receiving POLYDATA data type." << std::endl;

  // Create a message buffer to receive transform data
  igtl::PolyDataMessage::Pointer PolyData;
  PolyData = igtl::PolyDataMessage::New();
  PolyData->SetMessageHeader(header);
  PolyData->AllocatePack();
  
  // Receive transform data from the socket
  socket->Receive(PolyData->GetPackBodyPointer(), PolyData->GetPackBodySize());
  
  // Deserialize the transform data
  // If you want to skip CRC check, call Unpack() without argument.
  int c = PolyData->Unpack(1);
  
  if (c & igtl::MessageHeader::UNPACK_BODY) // if CRC check is OK
    {
    igtl::PolyDataPointArray::Pointer pointsArray        = PolyData->GetPoints();        
    igtl::PolyDataCellArray::Pointer verticesArray       = PolyData->GetVertices();      
    igtl::PolyDataCellArray::Pointer linesArray          = PolyData->GetLines();         
    igtl::PolyDataCellArray::Pointer polygonsArray       = PolyData->GetPolygons();      
    igtl::PolyDataCellArray::Pointer triangleStripsArray = PolyData->GetTriangleStrips();

    std::cout << "========== PolyData Contents ==========" << std::endl;
    if (pointsArray.IsNotNull())
      {
      std::cout << "  ------ Points ------" << std::endl;
      for (unsigned int i = 0; i < pointsArray->GetNumberOfPoints(); i ++)
        {
        igtlFloat32 point[3];
        pointsArray->GetPoint(i, point);
        std::cout << "  point[" << i << "] = (" << point[0] << ", "
                  << point[1] << ", " << point[2] << ")" << std::endl;
        }
      }

    if (verticesArray.IsNotNull())
      {
      std::cout << "  ------ Vertices ------" << std::endl;
      for (unsigned int i = 0; i < verticesArray->GetNumberOfCells(); i ++)
        {
        std::list<igtlUint32> cell;
        verticesArray->GetCell(i, cell);
        std::list<igtlUint32>::iterator iter;
        iter = cell.begin();
        if (iter != cell.end())
          {
          std::cout << "  cell[" << i << "] = (" << *iter;
          for (; iter != cell.end(); iter ++)
            {
            std::cout << ", " << *iter;
            }
          std::cout << ")" << std::endl;
          }
        }
      }

    if (linesArray.IsNotNull())
      {
      std::cout << "  ------ Lines ------" << std::endl;
      for (unsigned int i = 0; i < linesArray->GetNumberOfCells(); i ++)
        {
        std::list<igtlUint32> cell;
        linesArray->GetCell(i, cell);
        std::list<igtlUint32>::iterator iter;
        iter = cell.begin();
        if (iter != cell.end())
          {
          std::cout << "  cell[" << i << "] = (" << *iter;
          for (; iter != cell.end(); iter ++)
            {
            std::cout << ", " << *iter;
            }
          std::cout << ")" << std::endl;
          }
        }
      }

    if (polygonsArray.IsNotNull())
      {
      std::cout << "  ------ Polygons ------" << std::endl;
      for (unsigned int i = 0; i < polygonsArray->GetNumberOfCells(); i ++)
        {
        std::list<igtlUint32> cell;
        polygonsArray->GetCell(i, cell);
        std::list<igtlUint32>::iterator iter;
        iter = cell.begin();
        if (iter != cell.end())
          {
          std::cout << "  cell[" << i << "] = (" << *iter;
          for (; iter != cell.end(); iter ++)
            {
            std::cout << ", " << *iter;
            }
          std::cout << ")" << std::endl;
          }
        }
      }

    if (triangleStripsArray.IsNotNull())
      {
      std::cout << "  ------ TriangleStrips ------" << std::endl;
      for (unsigned int i = 0; i < triangleStripsArray->GetNumberOfCells(); i ++)
        {
        std::list<igtlUint32> cell;
        triangleStripsArray->GetCell(i, cell);
        std::list<igtlUint32>::iterator iter;
        iter = cell.begin();
        if (iter != cell.end())
          {
          std::cout << "  cell[" << i << "] = (" << *iter;
          for (; iter != cell.end(); iter ++)
            {
            std::cout << ", " << *iter;
            }
          std::cout << ")" << std::endl;
          }
        }
      }

    unsigned int nAttr = PolyData->GetNumberOfAttributes();

    for (unsigned int i = 0; i < nAttr; i ++)
      {
      std::cout << "  ------ Attributes #" << i << " ------" << std::endl;
      igtl::PolyDataAttribute * p = PolyData->GetAttribute(i);
      if (p)
        {
        std::cout << "  Name = " << p->GetName() << std::endl;
        std::cout << "  Type = ";
        switch (p->GetType())
          {
          case igtl::PolyDataAttribute::POINT_SCALAR:
            std::cout << "POINT_SCALAR" << std::endl;
            break;
          case igtl::PolyDataAttribute::POINT_VECTOR:
            std::cout << "POINT_VECTOR" << std::endl;
            break;
          case igtl::PolyDataAttribute::POINT_NORMAL:
            std::cout << "POINT_NORMAL" << std::endl;
            break;
          case igtl::PolyDataAttribute::POINT_TENSOR:
            std::cout << "POINT_TENSOR" << std::endl;
            break;
          case igtl::PolyDataAttribute::POINT_RGBA:
            std::cout << "POINT_RGBA" << std::endl;
            break;
          case igtl::PolyDataAttribute::CELL_SCALAR:
            std::cout << "CELL_SCALAR" << std::endl;
            break;
          case igtl::PolyDataAttribute::CELL_VECTOR:
            std::cout << "CELL_VECTOR" << std::endl;
            break;
          case igtl::PolyDataAttribute::CELL_NORMAL:
            std::cout << "CELL_NORMAL" << std::endl;
            break;
          case igtl::PolyDataAttribute::CELL_TENSOR:
            std::cout << "CELL_TENSOR" << std::endl;
            break;
          case igtl::PolyDataAttribute::CELL_RGBA:
            std::cout << "CELL_RGBA" << std::endl;
            break;
          }
        unsigned int size  = p->GetSize();
        unsigned int ncomp = p->GetNumberOfComponents();
        igtlFloat32 * data = new igtlFloat32[ncomp];
        for (unsigned int j = 0; j < size; j ++)
          {
          p->GetNthData(j, data);
          std::cout << "  data[" << j << "] = (" << data[0];
          for (unsigned int k = 1; k < ncomp; k ++)
            {
            std::cout << ", " << data[k];
            }
          std::cout << ")" << std::endl;
          }
        }
      }
    
    std::cout << "================================" << std::endl;
    return 1;
    }

  return 0;

}



