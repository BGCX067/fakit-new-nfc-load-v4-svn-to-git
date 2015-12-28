

unsigned int usCheckSum16( unsigned char * pucFrame, unsigned int usLen )
{
    unsigned int             iIndex;
    
    iIndex = 0;
    while( usLen-- )
    {
        iIndex += *( pucFrame++ );
    }
    return iIndex;
}
//        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
//        usCRC16 = usMBCRC16( ( UCHAR * ) pucSndBufferCur, usSndBufferCount );
//        ucRTUBuf[usSndBufferCount++] = ( UCHAR )( usCRC16 & 0xFF );
//        ucRTUBuf[usSndBufferCount++] = ( UCHAR )( usCRC16 >> 8 );


