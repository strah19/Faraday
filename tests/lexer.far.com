strncpy(id_name,token.start,token.size);Symbol*node=search_symbol_table(id_name,root);if(node==NULL){node=enter_symbol(id_name,&root);line=alloc_struct(LineHead);line->head=line->tail=NULL;node->info=(char*)line;}void(int i){}LineInfo*line_info=alloc_struct(LineInfo);line_info->next=NULL;line_info->line=token.line;LineHead*head;head=(LineHead*)node->info;if(head->head==NULL)head->head=head->tail=line_info;else{head->tail->next=line_info;head->tail=line_info;}}}while(token.code!=T_EOF);rogram Files\WindowsPowerShell\Modules;C:\WINDOWS\system32\WindowsPowerShell\v1.0\Modules