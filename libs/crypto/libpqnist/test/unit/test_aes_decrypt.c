/*
	Licensed to the Apache Software Foundation (ASF) under one
	or more contributor license agreements.  See the NOTICE file
	distributed with this work for additional information
	regarding copyright ownership.  The ASF licenses this file
	to you under the Apache License, Version 2.0 (the
	"License"); you may not use this file except in compliance
	with the License.  You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing,
	software distributed under the License is distributed on an
	"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
	KIND, either express or implied.  See the License for the
	specific language governing permissions and limitations
	under the License.
*/

/**
 * @file test_aes_decrypto.c
 * @author Kealan McCusker
 * @brief Test function for AES decryption,
 *
 * LICENSE
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <amcl/utils.h>
#include <amcl/randapi.h>
#include <pqnist/pqnist.h>

#define LINE_LEN 500
//#define DEBUG

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("usage: ./test_aes_decrypt [path to test vector file]\n");
        exit(EXIT_FAILURE);
    }

    bool readLine;
    FILE * fp = NULL;
    char line[LINE_LEN];
    char * linePtr = NULL;
    int l1=0;

    char * KEY = NULL;
    int KEYLen = 0;
    const char* KEYStr = "KEY = ";

    char * IV = NULL;
    int IVLen = 0;
    const char* IVStr = "IV = ";

    char * CIPHERTEXT = NULL;
    int CIPHERTEXTLen = 0;
    const char* CIPHERTEXTStr = "CIPHERTEXT = ";

    char * PLAINTEXT1 = NULL;
    const char* PLAINTEXTStr = "PLAINTEXT = ";
    const char* DECRYPTStr = "[DECRYPT]";

    // Open file
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("ERROR opening test vector file\n");
        exit(EXIT_FAILURE);
    }

    int lineNo=0;
    readLine = false;
    while ( (fgets(line, LINE_LEN, fp) != NULL))
    {
        if (!strncmp(line, DECRYPTStr,strlen(DECRYPTStr)))
        {
#ifdef DEBUG
            printf("line %d %s\n", lineNo,line);
#endif
            readLine = true;
        }

        if(!readLine)
            continue;

        if (!strncmp(line, KEYStr, strlen(KEYStr)))
        {
#ifdef DEBUG
            printf("line %d %s\n", lineNo,line);
#endif
            // Find hex value in string
            linePtr = line + strlen(KEYStr);

            // Allocate memory
            l1 = strlen(linePtr)-1;
            KEYLen = l1/2;
            KEY = (char*) malloc (KEYLen);
            if (KEY==NULL)
                exit(EXIT_FAILURE);

            // KEY binary value
            amcl_hex2bin(linePtr, KEY, l1);
        }

        if (!strncmp(line, IVStr, strlen(IVStr)))
        {
#ifdef DEBUG
            printf("line %d %s\n", lineNo,line);
#endif
            // Find hex value in string
            linePtr = line + strlen(IVStr);

            // Allocate memory
            l1 = strlen(linePtr)-1;
            IVLen = l1/2;
            IV = (char*) malloc (IVLen);
            if (IV==NULL)
                exit(EXIT_FAILURE);

            // IV binary value
            amcl_hex2bin(linePtr, IV, l1);
        }

        if (!strncmp(line, CIPHERTEXTStr, strlen(CIPHERTEXTStr)))
        {
#ifdef DEBUG
            printf("line %d %s\n", lineNo,line);
#endif
            // Find hex value in string
            linePtr = line + strlen(CIPHERTEXTStr);

            // Allocate memory
            l1 = strlen(linePtr)-1;
            CIPHERTEXTLen = l1/2;
            CIPHERTEXT = (char*) malloc (CIPHERTEXTLen);
            if (CIPHERTEXT==NULL)
                exit(EXIT_FAILURE);

            // CIPHERTEXT binary value
            amcl_hex2bin(linePtr, CIPHERTEXT, l1);
        }

        if (!strncmp(line, PLAINTEXTStr, strlen(PLAINTEXTStr)))
        {
#ifdef DEBUG
            printf("line %d %s\n", lineNo,line);
#endif
            // Find hex value in string
            linePtr = line + strlen(PLAINTEXTStr);

            // Allocate memory
            l1 = strlen(linePtr);
            PLAINTEXT1 = (char*) malloc(CIPHERTEXTLen+1);
            if (PLAINTEXT1==NULL)
                exit(EXIT_FAILURE);

            // Golden PLAINTEXT value
            octet PLAINTEXT1Oct= {CIPHERTEXTLen,CIPHERTEXTLen,PLAINTEXT1};
            amcl_hex2bin(linePtr, PLAINTEXT1, l1);

#ifdef DEBUG
            printf("KEY = ");
            amcl_print_hex(KEY, KEYLen);
            printf("IV = ");
            amcl_print_hex(IV, IVLen);
            printf("CIPHERTEXT = ");
            amcl_print_hex(CIPHERTEXT, CIPHERTEXTLen);
            printf("CIPHERTEXTLen = %d  blocks %0.2f \n", CIPHERTEXTLen, (float) CIPHERTEXTLen/16);
#endif

            // Decrypt
            pqnist_aes_cbc_decrypt(KEY, KEYLen, IV, CIPHERTEXT, CIPHERTEXTLen);
            octet PLAINTEXTOct= {CIPHERTEXTLen,CIPHERTEXTLen,CIPHERTEXT};

#ifdef DEBUG
            printf("PLAINTEXT = ");
            amcl_print_hex(CIPHERTEXT, CIPHERTEXTLen);
            printf("\n\n");
#endif

            int rc = OCT_comp(&PLAINTEXTOct,&PLAINTEXT1Oct);
            if (!rc)
            {
                printf("TEST AES DECRYPT FAILED COMPARE PLAINTEXT LINE %d\n",lineNo);
                exit(EXIT_FAILURE);
            }

            free(KEY);
            KEY = NULL;
            free(IV);
            IV = NULL;
            free(CIPHERTEXT);
            CIPHERTEXT = NULL;
            free(PLAINTEXT1);
            PLAINTEXT1 = NULL;
        }
        lineNo++;
    }
    fclose(fp);
    if (!readLine)
    {
        printf("ERROR Empty test vector file\n");
        exit(EXIT_FAILURE);
    }
    printf("SUCCESS TEST AES %s DECRYPT PASSED\n", argv[2]);
    exit(EXIT_SUCCESS);
}
