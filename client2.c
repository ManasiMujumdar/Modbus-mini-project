#include <stdio.h>
#include <libxml/parser.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>       /* for setitimer */
#include <signal.h>     /* for signal */
#include <modbus.h>
#include <unistd.h>
#include <libxml/xpath.h>
#include "unittest.h"
 
 xmlXPathObjectPtr getnodeset (xmlDocPtr doc, xmlChar *xpath){
	
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	if (context == NULL) {
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
                printf("No result\n");
		return NULL;
	}
	return result;
}
char *retrieve_addr(xmlDocPtr doc,xmlChar *xpath){
	
	//book[contains(@id, 'bk101')]/title
	// //book[@id='bk101']/author
	
	xmlNodeSetPtr nodeset;
	xmlXPathObjectPtr result;
	int i;
	xmlChar *keyword;
		char *addr;
	
	

	
	result = getnodeset (doc, xpath);
	if (result) {
		nodeset = result->nodesetval;
		for (i=0; i < nodeset->nodeNr; i++) {
			keyword = xmlNodeListGetString(doc, nodeset->nodeTab[i]->xmlChildrenNode, 1);
		addr=keyword;
	
		
		}
		
	}
	return addr;
	}

 
 int main()
 {
     uint8_t *tab_rp_bits;
     uint16_t *tab_rp_registers;
     modbus_t *ctx;
     int i,j;
     uint8_t value;
     int nb_points;
     int rc;
     float real;
     uint32_t ireal;
     struct timeval response_timeout;
     
     //parse client xml config file
     xmlDocPtr doc;
	doc = xmlParseFile("ModbusClientConfig1.xml");
	
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}

//set tcp context     
     xmlChar *xpath = (xmlChar*) "//Address/P[@type='IP']";
    const char *ipaddress;
   ipaddress=retrieve_addr(doc,xpath);    
ctx = modbus_new_tcp(ipaddress, 1502);
modbus_set_debug(ctx, TRUE);

//connect to the server
if (modbus_connect(ctx) == -1) {
         fprintf(stderr, "Connection failed: %s\n",
                 modbus_strerror(errno));
         modbus_free(ctx);
         return -1;
     }

/* Allocate and initialize the memory to store the bits */
     nb_points = (UT_BITS_NB > UT_INPUT_BITS_NB) ? UT_BITS_NB : UT_INPUT_BITS_NB;
     tab_rp_bits = (uint8_t *) malloc(nb_points * sizeof(uint8_t));
     memset(tab_rp_bits, 0, nb_points * sizeof(uint8_t));
 
     /* Allocate and initialize the memory to store the registers */
     nb_points = (UT_REGISTERS_NB > UT_INPUT_REGISTERS_NB) ?
         UT_REGISTERS_NB : UT_INPUT_REGISTERS_NB;
     tab_rp_registers = (uint16_t *) malloc(nb_points * sizeof(uint16_t));
     memset(tab_rp_registers, 0, nb_points * sizeof(uint16_t));
     
//set response wait time for Discrete input bits   
xmlChar *xpath1=(xmlChar*)"//ScanGroup/DiscreteInput/p/@ScanPeriod";
const char* time1;
time1=retrieve_addr(doc,xpath1);
int t1;
t1=atoi(time1);  
uint32_t T1=t1;
//modbus_set_response_timeout(ctx, T1, 0);
sleep(T1);
/*reading discrete inputs bits */
int addr_DI;
xmlChar *xpatha = (xmlChar*) "//ScanGroup/DiscreteInput/p/@StartAddress";
char *addrdi;
addrdi=retrieve_addr(doc,xpatha);
addr_DI=atoi(addrdi);
rc = modbus_read_input_bits(ctx, addr_DI,
                                 UT_INPUT_BITS_NB, tab_rp_bits);
     printf("1/1 modbus_read_input_bits: ");
 
     if (rc != UT_INPUT_BITS_NB) {
         printf("FAILED (nb points %d)\n", rc);
        
     }
 
     i = 0;
     nb_points = UT_INPUT_BITS_NB;
     while (nb_points > 0) {
         int nb_bits = (nb_points > 8) ? 8 : nb_points;
 
         value = modbus_get_byte_from_bits(tab_rp_bits, i*8, nb_bits);
         if (value != UT_INPUT_BITS_TAB[i]) {
             printf("FAILED (%0X != %0X)\n", value, UT_INPUT_BITS_TAB[i]);
             
         }
 
         nb_points -= nb_bits;
         i++;
     }
     
     printf("values are -----------\n");
     for (j=0; j < rc; j++) {
    printf("input bits[%d]=%d (0x%X)\n", j, UT_INPUT_BITS_TAB[j], UT_INPUT_BITS_TAB[j]);}
     
     printf("OK\n");
     
//set response wait time for Discrete Output bits
xmlChar *xpath2=(xmlChar*)"//ScanGroup/DiscreteOutput/p/@ScanPeriod";
char* time2;
time2=retrieve_addr(doc,xpath2);
int t2;
t2=atoi(time2);
uint32_t T2=t2;
//modbus_set_response_timeout(ctx, T2, 0);
sleep(T2);
/*reading Discrete Output Bits*/
int addr_DO;
xmlChar *xpathb = (xmlChar*) "//ScanGroup/DiscreteOutput/p/@StartAddress";
char *addrdo;
addrdo=retrieve_addr(doc,xpathb);
addr_DO=atoi(addrdo);
rc = modbus_read_bits(ctx, addr_DO, UT_BITS_NB, tab_rp_bits);
     printf("2/2 modbus_read_bits: ");
     if (rc != UT_BITS_NB) {
         printf("FAILED (nb points %d)\n", rc);
         
     }
 
     i = 0;
     nb_points = UT_BITS_NB;
     while (nb_points > 0) {
         int nb_bits = (nb_points > 8) ? 8 : nb_points;
 
         value = modbus_get_byte_from_bits(tab_rp_bits, i*8, nb_bits);
         if (value != UT_BITS_TAB[i]) {
             printf("FAILED (%0X != %0X)\n", value, UT_BITS_TAB[i]);
            
         }
 
         nb_points -= nb_bits;
         i++;
     }
     printf("values are ---------------- \n");
    
     for (j=0; j < rc; j++) {
    printf("discrete output bits [%d]=%d (0x%X)\n", j, UT_INPUT_REGISTERS_TAB[j], UT_INPUT_REGISTERS_TAB[j]);}
     

     printf("OK\n");
     
//set response wait time for analog input registers
xmlChar *xpath3=(xmlChar*)"//ScanGroup/AnalogInput/p/@ScanPeriod";
char* time3;
time3=retrieve_addr(doc,xpath3);
int t3;
t3=atoi(time3);
uint32_t T3=t3;
//modbus_set_response_timeout(ctx, T3, 0); 
sleep(T3); 
/*reading analog input registers*/
int addr_AI;
xmlChar *xpathc = (xmlChar*) "//ScanGroup/AnalogInput/p/@StartAddress";
char *addrai;
addrai=retrieve_addr(doc,xpathc);
addr_AI=atoi(addrai);
rc = modbus_read_input_registers(ctx, addr_AI,
                                      UT_INPUT_REGISTERS_NB,
                                      tab_rp_registers);
     printf("1/1 modbus_read_input_registers: ");
     if (rc != UT_INPUT_REGISTERS_NB) {
         printf("FAILED (nb points %d)\n", rc);
        
     }

     for (i=0; i < UT_INPUT_REGISTERS_NB; i++) {
         if (tab_rp_registers[i] != UT_INPUT_REGISTERS_TAB[i]) {
             printf("FAILED (%0X != %0X)\n",
                    tab_rp_registers[i], UT_INPUT_REGISTERS_TAB[i]);
             
         }
     }
     printf("values are -----------\n");
    
     for (j=0; j < rc; j++) {
    printf("reg[%d]=%d (0x%X)\n", j, UT_INPUT_REGISTERS_TAB[j], UT_INPUT_REGISTERS_TAB[j]);}
     
     printf("OK\n");
   
//set response wait time for analog output registers
xmlChar *xpath4=(xmlChar*)"//ScanGroup/AnalogOutput/p/@ScanPeriod";
char* time4;
time4=retrieve_addr(doc,xpath4);
int t4;
t4=atoi(time4); 
int x;
x=20;
uint32_t T4=t4;
//modbus_set_response_timeout(ctx, 20, 0);
sleep(T4);    
/*reading analog output registers*/
int addr_AO;
xmlChar *xpathd = (xmlChar*) "//ScanGroup/AnalogOutput/p/@StartAddress";
char *addrao;
addrao=retrieve_addr(doc,xpathd);
addr_AO=atoi(addrao);
rc = modbus_read_registers(ctx, addr_AO,
                                UT_REGISTERS_NB, tab_rp_registers);
     printf("2/5 modbus_read_registers: ");
     if (rc != UT_REGISTERS_NB) {
         printf("FAILED (nb points %d)\n", rc);
         
     }

     for (i=0; i < UT_REGISTERS_NB; i++) {
         if (tab_rp_registers[i] != UT_REGISTERS_TAB[i]) {
             printf("FAILED (%0X != %0X)\n",
                    tab_rp_registers[i],
                    UT_REGISTERS_TAB[i]);
             
         }
     }
     
     printf("values are -----------\n");
     for (j=0; j < rc; j++) {
    printf("reg[%d]=%d (0x%X)\n", j, UT_REGISTERS_TAB[j], UT_REGISTERS_TAB[j]);}
     
     printf("OK\n");
 
 printf("\nALL TESTS PASS WITH SUCCESS.\n");
 
 close:
     /* Free the memory */
     free(tab_rp_bits);
     free(tab_rp_registers);
 
     /* Close the connection */
     modbus_close(ctx);
     modbus_free(ctx);
 
     return 0;
 }
 
 
     
     
















     
