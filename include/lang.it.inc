<?php
// lang.it.inc
// italien language definition file
// $Id: lang.it.inc,v 1.2 2001/09/06 13:49:57 mb Exp $
//
// if you have suggestion for italian language please contact
// Luca Pescatore --> Luca Pescatore <l.pescatore@network.it>
//
// if you have a translation to another language, please contact
// me --> Matthias Blaser <mb@adfinis.ch>
//

// language definition
define("PQL_LANG","it");

// welcome texts (home.php)
define("PQL_WELCOME", "Benvenuto");
define("PQL_DESCRIPTION","Amministrazione di QMail-LDAP !!");

// general texts
define("PQL_SAVE", "Salva");
define("PQL_OPTIONS", "Opzioni");
define("PQL_EMAIL", "Email");
define("PQL_ACTIONS", "Azioni");

define("PQL_SURE", "Sei sicuro??");
define("PQL_YES", "Si...");
define("PQL_NO", "No");
define("PQL_ADD", "crea");

define("PQL_DOCUMENTATION", "Documentazione");
define("PQL_SHOW_CONFIG", "Show phpQLAdmin configuration"); // to translate
define("PQL_TEST_LDAP", "Test LDAP-Connection"); // to translate

// error messages
define("PQL_INVALID", "non valido");
define("PQL_MISSING", "mancante");
define("PQL_EXISTS", "esistente");
define("PQL_FAILED_RETYPE", "non inserita correttamente");
define("PQL_NO_OBJECT", "attributo non trovato");
define("PQL_GREATER_ZERO", "deve essere > 0");

// user/domain/address things
define("PQL_USER", "Utente");
define("PQL_USER_ID", "Nome Utente");
define("PQL_USER_DATA", "Informazioni Utente");
define("PQL_USER_DATA_SURNAME", "Cognome");
define("PQL_USER_DATA_NAME", "Nome");

define("PQL_USER_REGISTRED","Utente Registrato");
define("PQL_USER_NEW", "Registra un nuovo utente");
define("PQL_USER_DELETE", "Elimina utente");
define("PQL_USER_EDIT", "modifica informazioni");
define("PQL_USER_EXIST", "%user% esistente");
define("PQL_USER_NONE", "nessun utente registrato");
define("PQL_USER_MAILBOXPROPERTIES", "Proprieta' Mailbox");
define("PQL_USER_ADD_TITLE", "Crea utente per %domain%");
define("PQL_USER_ADD_HELP1", "La combinazione di nome e cognome (es. Zaphod Beeblebrox) devono essere univoche per l'intero dominio. Il nome utente deve essere univoco per tutto il dominio.");
define("PQL_USER_ADD_HELP2", "L'indirizzo email sara' convertito automaticamente in minuscolo");
define("PQL_USER_ADD_OK", "Nuovo utente creato con successo");
define("PQL_USER_ADD_FAILED", "Errore nella creazione di un nuovo utente");
define("PQL_USER_ACCOUNT_PROPERTIES", "Proprieta' Utente");
define("PQL_USER_ACCOUNT_PROPERTIES_MORE", "Proprieta' addizionali utente");
define("PQL_USER_ACCOUNT_TYPE", "Tipo");
define("PQL_USER_DEL_TITLE", "Rimuovi %user% da %domain%");
define("PQL_USER_DEL_OK", "Utente rimosso con successo");
define("PQL_USER_DEL_FAILED", "Errore nella rimozione dell'utente");
define("PQL_USER_DEL_ATTRIBUTE_TITLE", "Rimuovi attributo (%value%)");
define("PQL_USER_DEL_ATTRIBUTE_OK", "Attributo rimosso");
define("PQL_USER_DEL_ATTRIBUTE_FAILED", "Errore nella rimozione attributo");
define("PQL_USER_DELIVERYPROPERTIES","Advanced delivery properties"); // to translate

define("PQL_DNS_TITLE", "Informazioni DNS");
define("PQL_DNS_MAILHOST", "Nome Server Mail");
define("PQL_DNS_PRIORITY", "Priorita'");
define("PQL_DNS_NONE", "Il dominio non e' registrato sul DNS");

define("PQL_DOMAIN_CHANGE_ATTRIBUTE_TITLE", "Modifica i valori di tutti gli utenti del dominio");
define("PQL_DOMAIN_CHANGE_ATTRIBUTE", "Attributi");
define("PQL_DOMAIN_INVALID", "Dominio non valido!! utilizza: dominio.tld (Es. nomedominio.com)");
define("PQL_DOMAIN_EXISTS", "Dominio esistente");
define("PQL_DOMAIN_ADD", "Crea Dominio");
define("PQL_DOMAIN_ADD_OK", "%domain% creato con successo. <br> Ricordati che devi inserire manualmente il dominio nel file rpcthosts elocals in qmail/control !!");
define("PQL_DOMAIN_ADD_FAILED", "Errore nella creazione del dominio");
define("PQL_DOMAIN_DEL", "Rimuovi dominio");
define("PQL_DOMAIN_DEL_TITLE", "Rimuovi %domain%");
define("PQL_DOMAIN_DEL_WARNING", "<b>Attenzione:</b> Se elimini un dominio, automaticamente verranno eliminati tutte le utenze!!");
define("PQL_DOMAIN_DEL_OK", "Dominio rimosso con successo");
define("PQL_DOMAIN_DEL_FAILED", "Errore nella rimozione del dominio");

define("PQL_ADDRESS_REGISTRED", "Indirizzo Registrato");
define("PQL_ADDRESS_TYPE", "Tipo");

define("PQL_SENDMAIL", "Invia un mail di Prova");
define("PQL_SENDMAIL_OK", "Email inviata a %email% con successo");
define("PQL_SENDMAIL_FAILED", "Errore nell'invio della mail");

// deliverymode texts
// (normal, forwardonly, nombox, localdelivery, reply, echo)
define("PQL_LDAP_DELIVERYMODE_TITLE","Modalita' delivery");
define("PQL_LDAP_DELIVERYMODE_MODE","Modalita'");
define("PQL_LDAP_DELIVERYMODE_NORMAL","Normale");
define("PQL_LDAP_DELIVERYMODE_FORWARDONLY","Solo Forward");
define("PQL_LDAP_DELIVERYMODE_NOMBOX","Mailbox non locale");
define("PQL_LDAP_DELIVERYMODE_LOCALDELIVERY","Salva in locale");
define("PQL_LDAP_DELIVERYMODE_REPLY","Risposta Automatica");
define("PQL_LDAP_DELIVERYMODE_REPLY_SEND","Invia una risposta automatica");
define("PQL_LDAP_DELIVERYMODE_REPLY_TEXT","Testo per la risposta automatica");
define("PQL_LDAP_DELIVERYMODE_ECHO","Echo alla console ");
define("PQL_LDAP_DELIVERYMODE_CHANGE","Modifica proprieta'");
define("PQL_LDAP_DELIVERYMODE_CHANGE_OK","Modifica della modalita' diconsegna ");
define("PQL_LDAP_DELIVERYMODE_CHANGE_FAILED","Errore nella modifica della modalita' di consegna");
define("PQL_LDAP_DELIVERYMODE_NULL","Proprieta' di consegna");
define("PQL_LDAP_DELIVERYMODE_PROFILE", "Profilo");
define("PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL", "Normale(mailbox locale, account POP )");
define("PQL_LDAP_DELIVERYMODE_PROFILE_FORWARD", "Redirezione (solo redirezione, nessuna mailbox locale)");

// accountstatus texts
// (active, nopop, disable)
define("PQL_LDAP_ACCOUNTSTATUS_TITLE","Status Account");
define("PQL_LDAP_ACCOUNTSTATUS_STATUS","Status");
define("PQL_LDAP_ACCOUNTSTATUS_ACTIVE","attivo");
define("PQL_LDAP_ACCOUNTSTATUS_NOPOP","POP bloccato");
define("PQL_LDAP_ACCOUNTSTATUS_DISABLE","bloccato");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE","Setta lo status");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_OK","Status modificato in %status%");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_FAILED","Errore nella modifica dello status");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_ACTIVE","attivo");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_NOPOP","blocca l'account POP");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_DISABLE","blocca");

// mail texts
define("PQL_LDAP_MAIL_TITLE","Indirizzo principale");
define("PQL_LDAP_MAIL_CHANGE","Modifica l'indirizzo principale");
define("PQL_LDAP_MAIL_HELP", "L'indirizzo deve essere univoco per tutto il dominio");
define("PQL_LDAP_MAIL_CHANGE_OK", "Indirizzo principale modificato da %old% a %new%");
define("PQL_LDAP_MAIL_CHANGE_FAILED", "Errore nella modifica dell'indirizzo principale");
define("PQL_LDAP_MAIL_UNCHANGED", "Indirizzo principale non modificato");

// mailforwardingaddress texts
define("PQL_LDAP_MAILFORWARDINGADDRESS_TITLE","Redireziona");
define("PQL_LDAP_MAILFORWARDINGADDRESS_OTHER", "Redirezione ad altri utenti per questo indirizzo");
define("PQL_LDAP_MAILFORWARDINGADDRESS_NEW","Crea un nuovo indirizzo di redirezione");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_HELP", "Puoi inserire nuovi indirizzi di destinazione nel dettaglio dell'utente");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_OK", "Redirezione a %mail% creata con successo");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_FAILED", "Errore nella creazione della Redirezione");
define("PQL_LDAP_MAILFORWARDINGADDRESS_NONE","nessuno");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE", "Modifica l'indirizzo di redirezione");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE_OK", "Modifica della redirezione da %old% a %new%");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE_FAILED", "Errore nella modifica dell'indirizzo di redirezione");
define("PQL_LDAP_MAILFORWARDINGADDRESS_UNCHANGED", "Indirizzo di redirezione non modificato");

// mailalternateaddress texts
define("PQL_LDAP_MAILALTERNATEADDRESS_TITLE","Alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_NEW","Crea un nuovo alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_DEL", "Elimina alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_HELP", "L'alias deve essere univoco per tutto il dominio");
define("PQL_LDAP_MAILALTERNATEADDRESS_ADD_OK", "Alias (%mail%) creato con successo");
define("PQL_LDAP_MAILALTERNATEADDRESS_ADD_FAILED", "Errore nella creazione dell'alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE", "Modifica alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE_OK", "Modifica alias da %old% a %new%");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE_FAILED", "Errore nella modifica dell'alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_UNCHANGED","Alias non modificato");

// userpassword texts
define("PQL_LDAP_USERPASSWORD_TITLE", "Password"); // to translate
define("PQL_LDAP_USERPASSWORD_NONE", "none"); // to translate
define("PQL_LDAP_USERPASSWORD_ENCRYPTED", "encrypted"); // to translate
define("PQL_LDAP_USERPASSWORD_NEW", "Set password"); // to translate
define("PQL_LDAP_USERPASSWORD_RETYPE", "Password (Retype)"); // to translate
define("PQL_LDAP_USERPASSWORD_HELP", "A password can only contain numbers and letters, special characters are not allowed"); // to translate
define("PQL_LDAP_USERPASSWORD_CHANGE_OK", "Successfully changed password"); // to translate
define("PQL_LDAP_USERPASSWORD_CHANGE_FAILED", "Failed changing password"); // to translate

// uid texts
define("PQL_LDAP_UID_TITLE", "Nome Utente");
define("PQL_LDAP_UID_NONE", "nullo");
define("PQL_LDAP_UID_HELP", "Il nome utente puo' contenere solo numeri, lettere e i seguenti caratteri speciali &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;. In aggiunta, il nome utente deve essere univoco in tutto il sistema.");
define("PQL_LDAP_UID_HELP_SHORT", "Lettere, numeri e i seguenti caratteri speciali : &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;");
define("PQL_LDAP_UID_CHANGE", "Modifica Utente");
define("PQL_LDAP_UID_CHANGE_OK", "Utente modificato con successo da %old% a %new%");
define("PQL_LDAP_UID_CHANGE_FAILED", "Errore nella modifica dell'utente");
define("PQL_LDAP_UID_UNCHANGED","Utente non modificato");

// mailquota texts
define("PQL_LDAP_MAILQUOTA_TITLE", "Quota");
define("PQL_LDAP_MAILQUOTA_NONE", "Standard");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE", "massimo %maxsize% KB");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE_UNLIMITED", "illimitata");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS", "massimo %maxmails% email");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS_UNLIMITED", "email illimitate");
define("PQL_LDAP_MAILQUOTA_HELP", "Con 'Standard', qmail/ldap utilizza i quota dal file di configurazione");
define("PQL_LDAP_MAILQUOTA_NONESELECTED", "Devi selezionare almeno un tipo di quota");
define("PQL_LDAP_MAILQUOTA_MAXSIZE_TITLE", "Dimensione massima della mailbox");
define("PQL_LDAP_MAILQUOTA_MAXMAILS_TITLE", "Numero massimo di email");
define("PQL_LDAP_MAILQUOTA_DEFAULT", "Standard");
define("PQL_LDAP_MAILQUOTA_USERDEFINED", "Definito dall'utente");
define("PQL_LDAP_MAILQUOTA_CHANGE", "Modifica la quota");
define("PQL_LDAP_MAILQUOTA_CHANGE_OK", "Quota settata a %quota%");
define("PQL_LDAP_MAILQUOTA_CHANGE_FAILED", "Errore nella modifica della quotaquota");
define("PQL_LDAP_MAILQUOTA_UNCHANGED","Quota non modificata");

// mailhost texts
define("PQL_LDAP_MAILHOST_TITLE", "Server Mail");
define("PQL_LDAP_MAILHOST_FQDN", "Indirizzo Server (Nome Dominio)");
define("PQL_LDAP_MAILHOST_NONE", "nessuno");
define("PQL_LDAP_MAILHOST_CHANGE", "Modifica il nome del server di mail");
define("PQL_LDAP_MAILHOST_CHANGE_OK", "Modifica al server %mailhost% avvenuta con successo");
define("PQL_LDAP_MAILHOST_CHANGE_FAILED", "Errore nella modifica del mailserver");
define("PQL_LDAP_MAILHOST_DEFAULT", "Standard (utilizzo del DNS)");
define("PQL_LDAP_MAILHOST_USERDEFINED", "Definito dall'utente");
define("PQL_LDAP_MAILHOST_UNCHANGED", "Server Mail non modificato");
define("PQL_LDAP_MAILHOST_NOOPTION", "Nessuna opzione selezionata");
define("PQL_LDAP_MAILHOST_HELP", "Con l'opzione DNS, verra' utilizzato il primo record dell'MX del dominio");

// mailmessagestore texts
define("PQL_LDAP_MAILMESSAGESTORE_TITLE", "Path alla mailbox");
define("PQL_LDAP_MAILMESSAGESTORE_HELP", "Se non inserisci un path assoluto verra' preceduto dal valore presente in qmail/control/ldapmessagestore.'..' non sono consentiti.");
define("PQL_LDAP_MAILMESSAGESTORE_ABSOLUTE_NOTALLOWED", "I Path assoluti sono vietati dalla configurazione del sistema");
define("PQL_LDAP_MAILMESSAGESTORE_NONE", "nessuno");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE", "Modifica il path della mailbox");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE_OK", "Mailbox modificata con successo a %path%");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE_FAILED", "Errore nella modifica del path della mailbox");
define("PQL_LDAP_MAILMESSAGESTORE_UNCHANGED", "Mailbox non modificata");

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