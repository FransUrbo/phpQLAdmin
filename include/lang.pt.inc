<?php
// lang.pt.inc
// portuguese language definition file
// $Id: lang.pt.inc,v 1.1 2001/09/06 13:49:58 mb Exp $
//
// portugese language definition file
// by Alex A. Beeck <beeck@nttb.com.br>
//
// if you have a translation to another language, please contact
// me --> Matthias Blaser <mb@adfinis.ch>

// language definition
define("PQL_LANG","pt");

// welcome texts (home.php)
define("PQL_WELCOME", "Bemvindo a ");
define("PQL_DESCRIPTION","A Administração do QMail-LDAP !!");

// general texts
define("PQL_SAVE", "Salvar");
define("PQL_OPTIONS", "Opções");
define("PQL_EMAIL", "Correio");
define("PQL_ACTIONS", "Actions");

define("PQL_SURE", "Tem certeza ??");
define("PQL_YES", "Claro...");
define("PQL_NO", "Não");
define("PQL_ADD", "criar");

define("PQL_DOCUMENTATION", "Documentação");
define("PQL_SHOW_CONFIG", "Show phpQLAdmin configuration"); // to translate
define("PQL_TEST_LDAP", "Test LDAP-Connection"); // to translate

// error messages
define("PQL_INVALID", "inválido");
define("PQL_MISSING", "inexistente");
define("PQL_EXISTS", "já existe");
define("PQL_FAILED_RETYPE", "re-digitação difere");
define("PQL_NO_OBJECT", "atributo não encontrado");
define("PQL_GREATER_ZERO", "must be > 0");

// user/domain/address things
define("PQL_USER", "Usuário");
define("PQL_USER_ID", "Login");
define("PQL_USER_DATA", "Dados do Usuário");
define("PQL_USER_DATA_SURNAME", "Nome");
define("PQL_USER_DATA_NAME", "Sobrenome");

define("PQL_USER_REGISTRED","Usuário(s) Registrado(s)");
define("PQL_USER_NEW", "Registrar novo usuário");
define("PQL_USER_DELETE", "Remover usuário");
define("PQL_USER_EDIT", "Modificar dados do usuário");
define("PQL_USER_EXIST", "O usuário %user% já existe");
define("PQL_USER_NONE", "Não há usuários registrados");
define("PQL_USER_MAILBOXPROPERTIES", "Propriedades da Caixa Postal");
define("PQL_USER_ADD_TITLE", "Criar usuário para %domain%");
define("PQL_USER_ADD_HELP1", "A combinação de nome e sobrenome (ex. João da Silva) deve ser única em todo o domínio. O login deve ser único em todo o sistema.");
define("PQL_USER_ADD_HELP2", "O endereço de correio e o login serão convertidos automaticamente para letras minúsculas");
define("PQL_USER_ADD_OK", "O usuário novo foi criado com sucesso");
define("PQL_USER_ADD_FAILED", "Não foi possível criar o usuário");
define("PQL_USER_ACCOUNT_PROPERTIES", "Propriedades da conta");
define("PQL_USER_ACCOUNT_PROPERTIES_MORE", "Propriedades adicionais da conta");
define("PQL_USER_ACCOUNT_TYPE", "Tipo");
define("PQL_USER_DEL_TITLE", "Remover %user% de %domain%");
define("PQL_USER_DEL_OK", "O usuário foi removido com sucesso");
define("PQL_USER_DEL_FAILED", "Não foi possível remover o usuário");
define("PQL_USER_DEL_ATTRIBUTE_TITLE", "Remover atributo (%value%)");
define("PQL_USER_DEL_ATTRIBUTE_OK", "Atributo removido");
define("PQL_USER_DEL_ATTRIBUTE_FAILED", "Não foi possível remover o atributo");
define("PQL_USER_DELIVERYPROPERTIES","Advanced delivery properties"); // to translate

define("PQL_DNS_TITLE", "Informações de DNS");
define("PQL_DNS_MAILHOST", "Servidor de Correio");
define("PQL_DNS_PRIORITY", "Prioridade");
define("PQL_DNS_NONE", "Este domínio não está registrado dentro do DNS");

define("PQL_DOMAIN_CHANGE_ATTRIBUTE_TITLE", "Modificar atributos de todos os usuários deste domínio");
define("PQL_DOMAIN_CHANGE_ATTRIBUTE", "Atributo");
define("PQL_DOMAIN_INVALID", "Nome de domínio inválido !! use: dominio.tld (ex. adfinis.com)");
define("PQL_DOMAIN_EXISTS", "Este domínio já existe");
define("PQL_DOMAIN_ADD", "Criar Domínio");
define("PQL_DOMAIN_ADD_OK", "O domínio %domain% foi criado com sucesso. <br> Você terá que adicioná-lo
manualmente aos arquivos rcpthosts e locals no diretório qmail/control !!");
define("PQL_DOMAIN_ADD_FAILED", "Não foi possível criar o domínio");
define("PQL_DOMAIN_DEL", "Remover domínio");
define("PQL_DOMAIN_DEL_TITLE", "Remover domínio %domain%");
define("PQL_DOMAIN_DEL_WARNING", "<b>Atenção:</b> Se você remover um domínio, todos os usuários dentro deste domínio também serão removidos !!");
define("PQL_DOMAIN_DEL_OK", "O domínio foi removido com sucesso");
define("PQL_DOMAIN_DEL_FAILED", "Não foi possível remover o domínio");

define("PQL_ADDRESS_REGISTRED", "Endereços Registrados");
define("PQL_ADDRESS_TYPE", "Tipo");

define("PQL_SENDMAIL", "Enviar correio de teste");
define("PQL_SENDMAIL_OK", "Foi enviado um correio de teste para %email% com sucesso");
define("PQL_SENDMAIL_FAILED", "Não foi possível enviar um correio de teste");

// deliverymode texts
// (normal, forwardonly, nombox, localdelivery, reply, echo)
define("PQL_LDAP_DELIVERYMODE_TITLE","Modo de entrega");
define("PQL_LDAP_DELIVERYMODE_MODE","Modo");
define("PQL_LDAP_DELIVERYMODE_NORMAL","Normal");
define("PQL_LDAP_DELIVERYMODE_FORWARDONLY","Somente encaminhar");
define("PQL_LDAP_DELIVERYMODE_NOMBOX","Sem caixa postal local");
define("PQL_LDAP_DELIVERYMODE_LOCALDELIVERY","Armazenamento local");
define("PQL_LDAP_DELIVERYMODE_REPLY","Auto responder");
define("PQL_LDAP_DELIVERYMODE_REPLY_SEND","Enviar reposta automática");
define("PQL_LDAP_DELIVERYMODE_REPLY_TEXT","Texto de resposta");
define("PQL_LDAP_DELIVERYMODE_ECHO","Echoar no console");
define("PQL_LDAP_DELIVERYMODE_CHANGE","Modificar Propriedades");
define("PQL_LDAP_DELIVERYMODE_CHANGE_OK","Modo de entrega modificado");
define("PQL_LDAP_DELIVERYMODE_CHANGE_FAILED","Não foi possível modificar o modo de entrega");
define("PQL_LDAP_DELIVERYMODE_NULL","Propriedades padrão");
define("PQL_LDAP_DELIVERYMODE_PROFILE", "Perfil");
define("PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL", "Normal (caixa postal local, com conta POP)");
define("PQL_LDAP_DELIVERYMODE_PROFILE_FORWARD", "Encaminhar (somente encaminhar, sem conta POP)");

// accountstatus texts
// (active, nopop, disable)
define("PQL_LDAP_ACCOUNTSTATUS_TITLE","Status da conta");
define("PQL_LDAP_ACCOUNTSTATUS_STATUS","Status");
define("PQL_LDAP_ACCOUNTSTATUS_ACTIVE","ativa");
define("PQL_LDAP_ACCOUNTSTATUS_NOPOP","POP bloqueado");
define("PQL_LDAP_ACCOUNTSTATUS_DISABLE","bloqueado");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE","Definir Status");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_OK","Status definido como %status% com sucesso");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_FAILED","Não foi possível definir o status");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_ACTIVE","ativar");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_NOPOP","bloquear conta POP");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_DISABLE","bloquear");

// mail texts
define("PQL_LDAP_MAIL_TITLE","Endereço Principal");
define("PQL_LDAP_MAIL_CHANGE","Modificar Endereço Principal");
define("PQL_LDAP_MAIL_HELP", "O endereço deve ser único dentro de todo o domínio");
define("PQL_LDAP_MAIL_CHANGE_OK", "Endereço principal modificado de %old% para %new% com sucesso");
define("PQL_LDAP_MAIL_CHANGE_FAILED", "Não foi possível modificar o endereço principal");
define("PQL_LDAP_MAIL_UNCHANGED", "Endereço principal não modificado");

// mailforwardingaddress texts
define("PQL_LDAP_MAILFORWARDINGADDRESS_TITLE","Encaminhar");
define("PQL_LDAP_MAILFORWARDINGADDRESS_OTHER", "Encaminhamentos de outras contas para este usuário");
define("PQL_LDAP_MAILFORWARDINGADDRESS_NEW","Criar novo endereço para encaminhamento");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_HELP", "Você pode adicionar mais endereços para encaminhamento na página de detalhes do usuário");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_OK", "Encaminhamento para %mail% foi criado com sucesso");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_FAILED", "Não foi possível criar o encaminhamento");
define("PQL_LDAP_MAILFORWARDINGADDRESS_NONE","nenhum");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE", "Modificar endereço de encaminhamento");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE_OK", "Modificação do endereco de encaminhamento de %old% para %new% com sucesso");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE_FAILED", "Não foi possível modificar o endereço de encaminhamento");
define("PQL_LDAP_MAILFORWARDINGADDRESS_UNCHANGED", "Endereço de encaminhamento não modificado");

// mailalternateaddress texts
define("PQL_LDAP_MAILALTERNATEADDRESS_TITLE","Apelido");
define("PQL_LDAP_MAILALTERNATEADDRESS_NEW","Criar novo apelido");
define("PQL_LDAP_MAILALTERNATEADDRESS_DEL", "Remover apelido");
define("PQL_LDAP_MAILALTERNATEADDRESS_HELP", "Um apelido deve ser único em todo o domínio");
define("PQL_LDAP_MAILALTERNATEADDRESS_ADD_OK", "O apelido %mail% foi criado com sucesso");
define("PQL_LDAP_MAILALTERNATEADDRESS_ADD_FAILED", "Não foi possível criar o apelido");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE", "Modificar apelido");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE_OK", "Modificação do apelido %old% para %new% com sucesso");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE_FAILED", "Não foi possível modificar o apelido");
define("PQL_LDAP_MAILALTERNATEADDRESS_UNCHANGED","Apelido não modificado");

// userpassword texts
define("PQL_LDAP_USERPASSWORD_TITLE", "Senha");
define("PQL_LDAP_USERPASSWORD_NONE", "nenhuma");
define("PQL_LDAP_USERPASSWORD_ENCRYPTED", "criptografada");
define("PQL_LDAP_USERPASSWORD_NEW", "Definir senha");
define("PQL_LDAP_USERPASSWORD_RETYPE", "Senha (Redigitar)");
define("PQL_LDAP_USERPASSWORD_HELP", "Uma senha somente pode conter números e letras, caracteres especiais não são permitidos");
define("PQL_LDAP_USERPASSWORD_CHANGE_OK", "Senha modificada com sucesso");
define("PQL_LDAP_USERPASSWORD_CHANGE_FAILED", "Não foi possível modificar a senha");

// uid texts
define("PQL_LDAP_UID_TITLE", "Login");
define("PQL_LDAP_UID_NONE", "nenhum");
define("PQL_LDAP_UID_HELP", "O login somente pode conter letras, números e os seguintes caracteres especiais: The username can only contain numbers, letters and following special characters &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;. Além disso, o login deve ser único em todo o sistema.");
define("PQL_LDAP_UID_HELP_SHORT", "números, letras e os seguintes caracteres especiais: &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;");
define("PQL_LDAP_UID_CHANGE", "Modificar login");
define("PQL_LDAP_UID_CHANGE_OK", "Modificação do login %old% para %new% com sucesso");
define("PQL_LDAP_UID_CHANGE_FAILED", "Não foi possível modificar o login");
define("PQL_LDAP_UID_UNCHANGED","Login não modificado");

// mailquota texts
define("PQL_LDAP_MAILQUOTA_TITLE", "Quota");
define("PQL_LDAP_MAILQUOTA_NONE", "Padrão");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE", "max %maxsize% KB");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE_UNLIMITED", "tamanho ilimitado");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS", "max %maxmails% mensagens");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS_UNLIMITED", "número de mensagens ilimitado");
define("PQL_LDAP_MAILQUOTA_HELP", "Com o 'Padrão', qmail/ldap irá usar as quotas definidas nos seu arquivo de configuração");
define("PQL_LDAP_MAILQUOTA_NONESELECTED", "Você tem que selecionar ao menos um tipo de quota");
define("PQL_LDAP_MAILQUOTA_MAXSIZE_TITLE", "Tamanho máximo da caixa postal");
define("PQL_LDAP_MAILQUOTA_MAXMAILS_TITLE", "Número máximo de mensagens");
define("PQL_LDAP_MAILQUOTA_DEFAULT", "Padrão");
define("PQL_LDAP_MAILQUOTA_USERDEFINED", "Definido pelo usuário");
define("PQL_LDAP_MAILQUOTA_CHANGE", "Modificar quota");
define("PQL_LDAP_MAILQUOTA_CHANGE_OK", "A quota foi definida como %quota% com sucesso");
define("PQL_LDAP_MAILQUOTA_CHANGE_FAILED", "Não foi possível modificar a quota");
define("PQL_LDAP_MAILQUOTA_UNCHANGED","Quota não modificada");

// mailhost texts
define("PQL_LDAP_MAILHOST_TITLE", "Servidor de correio");
define("PQL_LDAP_MAILHOST_FQDN", "Endereço do servidor");
define("PQL_LDAP_MAILHOST_NONE", "nenhum");
define("PQL_LDAP_MAILHOST_CHANGE", "Modificar endereço do servidor de correio");
define("PQL_LDAP_MAILHOST_CHANGE_OK", "Endereço do servidor de correio modificado para %mailhost% com sucesso");
define("PQL_LDAP_MAILHOST_CHANGE_FAILED", "Não foi possível modificar o endereço do servidor de correio");
define("PQL_LDAP_MAILHOST_DEFAULT", "Padrão (definido no DNS)");
define("PQL_LDAP_MAILHOST_USERDEFINED", "Definido pelo usuário");
define("PQL_LDAP_MAILHOST_UNCHANGED", "Endereço do servidor não modificado");
define("PQL_LDAP_MAILHOST_NOOPTION", "Nenhuma opção foi selecionada");
define("PQL_LDAP_MAILHOST_HELP", "Com a opção DNS, o primeiro registro MX deste domínio será utilizado");

// mailmessagestore texts
define("PQL_LDAP_MAILMESSAGESTORE_TITLE", "Caminho para a caixa postal");
define("PQL_LDAP_MAILMESSAGESTORE_HELP", "Se você não providenciar um caminho absoluto, ele será prefixado com o valor definido em qmail/control/ldapmessagestore. '..' não são permitidos.");
define("PQL_LDAP_MAILMESSAGESTORE_ABSOLUTE_NOTALLOWED", "Caminhos absolutos bloqueados pelo sistema");
define("PQL_LDAP_MAILMESSAGESTORE_NONE", "nenhum");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE", "Modificar caminho para a caixa postal");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE_OK", "Caminho para a caixa postal modificado para %path% com sucesso");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE_FAILED", "Não foi possível modificar o caminho para a caixa postal");
define("PQL_LDAP_MAILMESSAGESTORE_UNCHANGED", "Caminho para a caixa postal não modificado");

// qmaildotmode texts
define("PQL_LDAP_QMAILDOTMODE_TITLE", "dotmode"); // to translate
define("PQL_LDAP_QMAILDOTMODE_HELP", "If you set dotmode to both or ldapwithprog you should add a deliveryprogram too");
define("PQL_LDAP_QMAILDOTMODE_NONE", "default"); // to translate
define("PQL_LDAP_QMAILDOTMODE_CHANGE", "Change dotmode"); // to translate
define("PQL_LDAP_QMAILDOTMODE_CHANGE_OK", "Successfully changed dotmode to %mode%"); // to translate
define("PQL_LDAP_QMAILDOTMODE_CHANGE_FAILED", "Failed changing dotmode"); // to translate
define("PQL_LDAP_QMAILDOTMODE_UNCHANGED", "dotmode unchanged"); // to translate
define("PQL_LDAP_QMAILDOTMODE_MODE_DEFAULT", "default (taken from ldapdefaultdotmode)"); // to translate
define("PQL_LDAP_QMAILDOTMODE_MODE_BOTH", "both (deliveryProgramPath + .qmail)"); // to translate
define("PQL_LDAP_QMAILDOTMODE_MODE_LDAPONLY", "ldaponly (deliveryProgramPath + .qmail ignored for ldap deliveries)"); // to translate
define("PQL_LDAP_QMAILDOTMODE_MODE_LDAPWITHPROG", "ldapwithprog (deliveryProgramPath used, .qmail ignored)"); // to translate
define("PQL_LDAP_QMAILDOTMODE_MODE_NONE", "none (deliveryProgramPath + .qmail ignored)"); // to translate

// deliveryprogrampath texts
define("PQL_LDAP_DELIVERYPROGRAMPATH_TITLE", "Deliveryprogram"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_HELP", "This is the same as '|/path/to/someprog' in a .qmail file. Only used if qmailDotMode is set to ldapwithprog or both."); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_NONE", "none"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE", "Change path to deliveryprogram"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_OK", "Successfully changed path to %path%"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_FAILED", "Failed changing path"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_UNCHANGED", "deliveryprogram path unchanged"); // to translate
define("PQL_LDAP_DELIVERYPROGRAMPATH_DOTWARNING", "The qmaildotmode of this user is set to %qmaildotmode%. Please to set it to both or ldapwithprog to get the deliveryprogram running."); // to translate

// search engine texts
define("PQL_SEARCH_TITLE","Find Users Whose"); // to translate
define("PQL_SEARCH_TITLE2","Search Results"); // to translate
define("PQL_SEARCH_UID","Username"); // to translate
define("PQL_SEARCH_CN","Common Name"); // to translate
define("PQL_SEARCH_MAIL","Mail Address"); // to translate
define("PQL_SEARCH_MAILALTERNATEADDRESS","Mail Alias"); // to translate
define("PQL_SEARCH_MAILFORWARDINGADDRESS","Forwarding Address"); // to translate
define("PQL_SEARCH_CONTAINS","Contains"); // to translate
define("PQL_SEARCH_IS","Is"); // to translate
define("PQL_SEARCH_STARTSWITH","Starts With"); // to translate
define("PQL_SEARCH_ENDSWITH","Ends With"); // to translate
define("PQL_SEARCH_FINDBUTTON","find"); // to translate

// ldap connection test texts
define("PQL_TEST_TITLE", "phpQLAdmin ldap connection"); // to translate
define("PQL_TEST_TESTS", "Tests"); // to translate
define("PQL_TEST_HELP", "This is a simple test to show if the ldap extension is loaded and the connections are working. It does not provide tests on access rights in the directory."); // to translate
define("PQL_TEST_LDAP_EXT_TITLE", "php ldap extension"); // to translate
define("PQL_TEST_LDAP_EXT_NA", "not available"); // to translate
define("PQL_TEST_LDAP_EXT_OK", "built in (loaded)"); // to translate
define("PQL_TEST_CONNECTION_FAILED", "failed"); // to translate
define("PQL_TEST_CONNECTION_RESOLVE_ERR", "the hostname %host% could not be resolved"); // to translate
define("PQL_TEST_CONNECTION_CONNECT_ERR", "could not connect to port 389 at %host%, please make sure the service is up and it is not blocked with a firewall"); // to translate
define("PQL_TEST_CONNECTION_CONNECT_OK", "ok, have fun ;-)"); // to translate
define("PQL_TEST_CONNECTION_USER_TITLE", "user directory connection"); // to translate
define("PQL_TEST_CONNECTION_USER_BIND_ERR", "connection ok, but could not bind to the directory.");
define("PQL_TEST_CONNECTION_CONROL_TITLE", "control directory connection"); // to translate
define("PQL_TEST_CONNECTION_CONTROL_BIND_ERR", "connection ok, but could not bind to the directory. please check PQL_LDAP_CONTROL_ROOTDN and PQL_LDAP_CONTROL_ROOTPW"); // to translate
define("PQL_TEST_CONNECTION_CONROL_INACTIVE", "control extension deactivated"); // to translate
?>