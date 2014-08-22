#include "CommonHeader.h"

BitmapFont::BitmapFont(char* filename)
{
    m_Initialized = false;

    m_ImageName[0] = 0;
    m_Chars = 0;
    m_KerningPairs = 0;
    m_NumChars = 0;
    m_NumKerningPairs = 0;

	m_XOffset = 0;

    int length;
    char* buffer = LoadFile( filename, &length, false );
    ReadFont( buffer, length );
}

BitmapFont::~BitmapFont()
{
    if( m_Chars )
        delete[] m_Chars;
    if( m_KerningPairs )
        delete[] m_KerningPairs;
}

void BitmapFont::ReadFont(char* filebuffer, unsigned int buffersize)
{
    ErrorLog( "ReadFont - %d, %c%c%c%c\n", buffersize, filebuffer[0], filebuffer[1], filebuffer[2], filebuffer[3] );

    assert( filebuffer[0] == 'B' );
    assert( filebuffer[1] == 'M' );
    assert( filebuffer[2] == 'F' );
    assert( filebuffer[3] == 3 ); // This code only reads version 3 files... update it if using a newer BitmapFont format.

    char* fileptr = filebuffer + 4;

    while( fileptr < filebuffer + buffersize )
    {
        switch( fileptr[0] )
        {
        case 1: // Block type 1: info
            {
                fileptr += 1; // skip past the block type.
                unsigned int blocksize = *(unsigned int*)fileptr;
                fileptr += 4; // skip past size;
            
                // ignoring this block
                fileptr += blocksize;
            }
            break;

        case 2: //Block type 2: common
            {
                fileptr += 1; // skip past the block type.
                //unsigned int blocksize = *(unsigned int*)fileptr;
                fileptr += 4; // skip past size;

                m_Properties.lineHeight = *(unsigned short*)fileptr;
                fileptr += sizeof( m_Properties.lineHeight );

                m_Properties.base = *(unsigned short*)fileptr;
                fileptr += sizeof( m_Properties.base );

                m_Properties.scaleW = *(unsigned short*)fileptr;
                fileptr += sizeof( m_Properties.scaleW );

                m_Properties.scaleH = *(unsigned short*)fileptr;
                fileptr += sizeof( m_Properties.scaleH );

                m_Properties.pages = *(unsigned short*)fileptr;
                fileptr += sizeof( m_Properties.pages );

                unsigned char bitfields = *(unsigned char*)fileptr; // bits 0-6: reserved, bit 7: packed
                m_Properties.packed = (bitfields & 1<<7) > 0 ? true : false;
                fileptr += sizeof( bitfields );

                m_Properties.alphaChnl = *(unsigned char*)fileptr;
                fileptr += sizeof( m_Properties.alphaChnl );

                m_Properties.redChnl = *(unsigned char*)fileptr;
                fileptr += sizeof( m_Properties.redChnl );

                m_Properties.greenChnl = *(unsigned char*)fileptr;
                fileptr += sizeof( m_Properties.greenChnl );

                m_Properties.blueChnl = *(unsigned char*)fileptr;
                fileptr += sizeof( m_Properties.blueChnl );
            }
            break;

        case 3: // Block type 3: pages
            {
                fileptr += 1; // skip past the block type.
                unsigned int blocksize = *(unsigned int*)fileptr;
                fileptr += 4; // skip past size;
            
                // only readying the first file, ignoring others.
                strcpy_s( m_ImageName, MAX_PATH, fileptr );
                fileptr += blocksize;
            }
            break;

        case 4: // Block type 4: chars
            {
                fileptr += 1; // skip past the block type.
                unsigned int blocksize = *(unsigned int*)fileptr;
                fileptr += 4; // skip past size;

                m_NumChars = blocksize / 20;

                m_Chars = new CharDescriptor[m_NumChars];

                for( unsigned int i=0; i<m_NumChars; i++ )
                {
                    m_Chars[i].id = *(unsigned int*)fileptr;
                    fileptr += sizeof( m_Chars[i].id );

                    m_Chars[i].x = *(unsigned short*)fileptr;
                    fileptr += sizeof( m_Chars[i].x );

                    m_Chars[i].y = *(unsigned short*)fileptr;
                    fileptr += sizeof( m_Chars[i].y );

                    m_Chars[i].width = *(unsigned short*)fileptr;
                    fileptr += sizeof( m_Chars[i].width );

                    m_Chars[i].height = *(unsigned short*)fileptr;
                    fileptr += sizeof( m_Chars[i].height );

                    m_Chars[i].xoffset = *(short*)fileptr;
                    fileptr += sizeof( m_Chars[i].xoffset );

                    m_Chars[i].yoffset = *(short*)fileptr;
                    fileptr += sizeof( m_Chars[i].yoffset );

                    m_Chars[i].xadvance = *(short*)fileptr;
                    fileptr += sizeof( m_Chars[i].xadvance );

                    m_Chars[i].page = *(unsigned char*)fileptr;
                    fileptr += sizeof( m_Chars[i].page );

                    m_Chars[i].chnl = *(unsigned char*)fileptr;
                    fileptr += sizeof( m_Chars[i].chnl );
                }
            }
            break;

        case 5: // Block type 5: kerning pairs
            {
                fileptr += 1; // skip past the block type.
                unsigned int blocksize = *(unsigned int*)fileptr;
                fileptr += 4; // skip past size;
            
                m_NumKerningPairs = blocksize / 10;

                m_KerningPairs = new KerningPair[m_NumKerningPairs];

                for( unsigned int i=0; i<m_NumKerningPairs; i++ )
                {
                    m_KerningPairs[i].first = *(unsigned int*)fileptr;
                    fileptr += sizeof( m_KerningPairs[i].first );

                    m_KerningPairs[i].second = *(unsigned int*)fileptr;
                    fileptr += sizeof( m_KerningPairs[i].second );

                    m_KerningPairs[i].amount = *(short*)fileptr;
                    fileptr += sizeof( m_KerningPairs[i].amount );
                }
            }
            break;
        }
    }

    ErrorLog( "Font Initialized - %s with %d chars.\n", m_ImageName, m_NumChars );
    m_Initialized = true;
}

int BitmapFont::GenerateVerts(char* string, VertDef* verts, GLushort* indices, float height, int justificationflags, Vector3 color)
{
	for( int j=0; j<strlen(string)*6; j++ )
    {
		verts[j].pos[2] = 0;
        verts[j].col[0] = color.x;
        verts[j].col[1] = color.y;
        verts[j].col[2] = color.z;
        verts[j].col[3] = 1;
    }

	m_XOffset = 0;
	m_YOffset = 0;
	float scale = height/m_Properties.lineHeight;

	CharDescriptor* cc;

	for(int stringPos = 0; stringPos < strlen(string); stringPos++)
	{
		for(int charPos = 0; charPos < m_NumChars; charPos++)
		{
			if(string[stringPos] == m_Chars[charPos].id)
			{
				cc = &m_Chars[charPos];
			}
		}

		if(m_YOffset < cc->height)
			m_YOffset = cc->height;

		float texw = m_Properties.scaleW;
		float texh = m_Properties.scaleH;

		float uleft = (float)cc->x / texw;
		float uright = ((float)cc->x + cc->width) / texw;

		float vtop = (texh - cc->y) / texh;
		float vbottom = (texh - cc->y - cc->height) / texh;

		// upper left
		verts[stringPos*4].pos[0] = (m_XOffset) * scale;
		verts[stringPos*4].pos[1] = (cc->height) * scale;
		verts[stringPos*4].uv[0] = uleft;
		verts[stringPos*4].uv[1] = vtop;

		// upper right
		verts[stringPos*4 + 1].pos[0]= (m_XOffset + cc->width) * scale;
		verts[stringPos*4 + 1].pos[1] = (cc->height ) * scale;
		verts[stringPos*4 + 1].uv[0]  = uright;
		verts[stringPos*4 + 1].uv[1]  = vtop;

		// lower left
		verts[stringPos*4 + 2].pos[0] = (m_XOffset) * scale;
		verts[stringPos*4 + 2].pos[1] = 0;
		verts[stringPos*4 + 2].uv[0]  = uleft;
		verts[stringPos*4 + 2].uv[1]  = vbottom;

		// lower right
		verts[stringPos*4 + 3].pos[0]= (m_XOffset + cc->width) * scale;
		verts[stringPos*4 + 3].pos[1]= 0;
		verts[stringPos*4 + 3].uv[0] = uright;
		verts[stringPos*4 + 3].uv[1] = vbottom;

		indices[stringPos*6] =   stringPos*4;
		indices[stringPos*6+1] = stringPos*4+1;
		indices[stringPos*6+2] = stringPos*4+2;
		indices[stringPos*6+3] = stringPos*4+1;
		indices[stringPos*6+4] = stringPos*4+2;
		indices[stringPos*6+5] = stringPos*4+3;

		if(cc->id == 121 || cc->id == 112 || cc->id == 113 || cc->id == 103 || cc->id == 106)
		{
			verts[stringPos*4].pos[1]-= cc->yoffset * scale;
			verts[stringPos*4 + 1].pos[1] -= cc->yoffset * scale;
			verts[stringPos*4 + 2].pos[1] -= cc->yoffset* scale;
			verts[stringPos*4 + 3].pos[1] -= cc->yoffset * scale;
		}

		m_XOffset += cc->width;
	}

	for(int i = 0; i < 6*strlen(string); i++)
	{
		if( justificationflags & Justify_Right )
		{
			verts[i].pos[0] -= m_XOffset * scale;
		}

		if( justificationflags & Justify_Top )
		{
			verts[i].pos[1] -= m_YOffset * scale;
		}

		if( justificationflags & Justify_CenterX )
		{
			verts[i].pos[0] -= (m_XOffset/2) * scale;
		}

		if( justificationflags & Justify_CenterY )
		{
			verts[i].pos[1] -= m_YOffset/2 * scale; 
		}
	}

    return 6*strlen(string);
}
