#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
int Parse_XML_File(char *buf, int len) {
	xmlDocPtr doc;
	xmlNodePtr root, node, detail;
	xmlChar *name, *value;
	doc = xmlReadFile("test.xml", "GB2312", XML_PARSE_RECOVER);  //parse xml in memory
	if(doc == NULL) {
		printf("Open XML file error!\n");
		return -1;
	}
	/*Find Node called content*/
	root = xmlDocGetRootElement(doc);//找出根节点 SAIC:ECUProgrammingProcessFile
	if (root == NULL)   
	{
		printf("No root node error in xml\n"); 
		return -1;
	}
	for (node = root->children; node; node = node->next)//遍历根节点的子节点 
	{
		if (xmlStrcasecmp(node->name, BAD_CAST"Header") == 0)
		{
			for (node = node->children; node; node = node->next)
			{
				if (xmlStrcasecmp(node->name, BAD_CAST"ECUInformation") == 0)
				{
					for (node = node->children; node; node = node->next)
					{
						if (xmlStrcasecmp(node->name, BAD_CAST"CP_Can_PhysReqId") == 0) 
						{
							value = xmlNodeGetContent(node);
							printf("The Req Id %s:\n", (char*)value);
							xmlFree(value);
						}
						if (xmlStrcasecmp(node->name, BAD_CAST"CP_Can_RespUSDTId") == 0) 
						{
							value = xmlNodeGetContent(node);
							printf("The Req Id %s:\n", (char*)value);
							xmlFree(value);
						}
						if (xmlStrcasecmp(node->name, BAD_CAST"CP_FuncReqId") == 0) 
						{
							value = xmlNodeGetContent(node);
							printf("The Req Id %s:\n", (char*)value);
							xmlFree(value);
						}
					}	
				}
				if (xmlStrcasecmp(node->name, BAD_CAST"CommunicationParameter") == 0)
				{
					for (node = node->children; node; node = node->next)
					{
						if (xmlStrcasecmp(node->name, BAD_CAST"CP_Baudrate") == 0) 
						{
							value = xmlNodeGetContent(node);
							printf("The Req Id %s:\n", (char*)value);
							xmlFree(value);
						}
						if (xmlStrcasecmp(node->name, BAD_CAST"CP_P2") == 0) 
						{
							value = xmlNodeGetContent(node);
							printf("The Req Id %s:\n", (char*)value);
							xmlFree(value);
						}
						if (xmlStrcasecmp(node->name, BAD_CAST"CP_P2Star") == 0) 
						{
							value = xmlNodeGetContent(node);
							printf("The Req Id %s:\n", (char*)value);
							xmlFree(value);
						}
						if (xmlStrcasecmp(node->name, BAD_CAST"CP_P3Min") == 0) 
						{
							value = xmlNodeGetContent(node);
							printf("The Req Id %s:\n", (char*)value);
							xmlFree(value);
						}
					}	
				}
		
			}
		}
		/*else if (xmlStrcasecmp(node->name, BAD_CAST"DOPS") == 0)
		{
			for (node = node->children; node; node = node->next)
			{
				if (xmlStrcasecmp(node->name, BAD_CAST"SystemDefined") == 0)
				{
					for (node = node->children; node; node = node->next)
					{
						if (xmlStrcasecmp(node->name, BAD_CAST"DOP") == 0) 
						{	
							for (detail = node->children; detail; detail = detail->next)
								if (xmlStrcasecmp(node->name, BAD_CAST"DOP") == 0)
								{
									name = xmlGetProp(detail, BAD_CAST"DefaultValue");
									value = xmlNodeGetContent(node);
									printf("The Req Id %s:\n", (char*)value);
									xmlFree(value);
								}
							
						}
					}	
				}
				else if (xmlStrcasecmp(node->name, BAD_CAST"UserDefined") == 0)
				{
					
				}
				
			}
		
		}
		*/
		else if (xmlStrcasecmp(node->name, BAD_CAST"ProgrammingProcess") == 0)
		{
			
		
		}
		
	}
	
	xmlFreeDoc(doc);
	return 0;
}
int main()
{
	char *content;
	unsigned long filesize;
	FILE *file;
	if ((file = fopen("test.xml", "r")) == NULL) {
		perror("openf file error");
	}
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	rewind(file);
	content = (char *)malloc(filesize + 1);
	memset(content, 0, filesize + 1);
	fread(content, 1, filesize, file);
	fclose(file);
	printf("content:\n%s\n", content);
	if (Parse_XML_File(content, filesize,) < 0) {
		perror("parse xml failed");
	}
	return 0;

}