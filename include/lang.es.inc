<?php
// lang.es.inc
// spanish language definition file
// $Id: lang.es.inc,v 1.1 2001/11/02 13:38:02 mb Exp $
//
// spanish language definition file
// by Pablo Suau <psuau@renr.es>
//
// if you have a translation to another language, please contact
// me --> Turbo Fredriksson <phpqladmin@bayour.com>

// language definition
define("PQL_LANG_LANG","es");

// welcome texts (home.php)
define("PQL_LANG_WELCOME", "Bienvenido a ");
define("PQL_LANG_DESCRIPTION","Administración QMail-LDAP !!");

// general texts
define("PQL_LANG_SAVE", "Guardar");
define("PQL_LANG_OPTIONS", "Opciones");
define("PQL_LANG_EMAIL", "Email");
define("PQL_LANG_ACTIONS", "Acciones");

define("PQL_LANG_SURE", "Estas realmente seguro ??");
define("PQL_LANG_YES", "Si...");
define("PQL_LANG_NO", "No");
define("PQL_LANG_ADD", "crear");

define("PQL_LANG_DOCUMENTATION", "Documentación");
define("PQL_LANG_SHOW_CONFIG", "Mostrar la configuración de phpQLAdmin");
define("PQL_LANG_TEST_LDAP", "Probar la conexión LDAP");

// error messages
define("PQL_LANG_INVALID", "inválido");
define("PQL_LANG_MISSING", "falta");
define("PQL_LANG_EXISTS", "ya existe");
define("PQL_LANG_FAILED_RETYPE", "no repetido de forma exacta");
define("PQL_LANG_NO_OBJECT", "atributo no encontrado");
define("PQL_LANG_GREATER_ZERO", "debe ser > 0");

// user/domain/address things
define("PQL_LANG_USER", "Usuario");
define("PQL_LANG_USER_ID", "Nombre de usuario");
define("PQL_LANG_USER_DATA", "Datos del usuario");
define("PQL_LANG_USER_DATA_SURNAME", "Apellido");
define("PQL_LANG_USER_DATA_NAME", "Nombre");

define("PQL_LANG_USER_REGISTRED","Usuario registrado");
define("PQL_LANG_USER_NEW", "Registrar nuevo usuario");
define("PQL_LANG_USER_DELETE", "Borrar usuario");
define("PQL_LANG_USER_EDIT", "Cambiar datos del usuario");
define("PQL_LANG_USER_EXIST", "El usuario %user% ya existe");
define("PQL_LANG_USER_NONE", "Ningún usuario registrado");
define("PQL_LANG_USER_MAILBOXPROPERTIES", "Propiedades del mailbox");
define("PQL_LANG_USER_ADD_TITLE", "Crear usuario en %domain%");
define("PQL_LANG_USER_ADD_HELP1", "La combinación de apeelido y nombre (p.ej. Pablo Suau) debe ser única en tidi ek dominio. El nombre de usuario debe ser único en todo el sistema.");
define("PQL_LANG_USER_ADD_HELP2", "La dirección de correo electrónico y el nombre de usuario serán automáticamente convertidos a minúsculas");
define("PQL_LANG_USER_ADD_OK", "Nuevo usuario creado con éxito");
define("PQL_LANG_USER_ADD_FAILED", "Se falló al crear el nuevo usuario");
define("PQL_LANG_USER_ACCOUNT_PROPERTIES", "Propiedades de la cuenta");
define("PQL_LANG_USER_ACCOUNT_PROPERTIES_MORE", "Propiedades adicionales de la cuenta");
define("PQL_LANG_USER_ACCOUNT_TYPE", "Tipo");
define("PQL_LANG_USER_DEL_TITLE", "Eliminar %user% de %domain%");
define("PQL_LANG_USER_DEL_OK", "Usuario eliminado con éxito");
define("PQL_LANG_USER_DEL_FAILED", "Se falló al eliminar el usuario");
define("PQL_LANG_USER_DEL_ATTRIBUTE_TITLE", "Eliminar atributo (%value%)");
define("PQL_LANG_USER_DEL_ATTRIBUTE_OK", "Atributo eliminado");
define("PQL_LANG_USER_DEL_ATTRIBUTE_FAILED", "Se falló al eliminar el atributo");
define("PQL_LANG_USER_DELIVERYPROPERTIES","Propiedades avanzadas de entrega");

define("PQL_LANG_DNS_TITLE", "Informaciones DNS");
define("PQL_LANG_DNS_MAILHOST", "Mailhost");
define("PQL_LANG_DNS_PRIORITY", "Prioridad");
define("PQL_LANG_DNS_NONE", "Este dominio no está registrado dentro del DNS");

define("PQL_LANG_DOMAIN_CHANGE_ATTRIBUTE_TITLE", "Cambiar los valores de todos los usuarios de este dominio");
define("PQL_LANG_DOMAIN_CHANGE_ATTRIBUTE", "Atributo");
define("PQL_LANG_DOMAIN_INVALID", "Nombre de dominio inválido !! uso: domain.tld (p.ej. renr.es)");
define("PQL_LANG_DOMAIN_EXISTS", "Este dominio ya existe");
define("PQL_LANG_DOMAIN_ADD", "Crear dominio");
define("PQL_LANG_DOMAIN_ADD_OK", "Dominio %domain% creado con éxito. <br> Debes añadir manualmente este dominio a los archivos rpcthosys y locals en el directorio qmail/control !!");
define("PQL_LANG_DOMAIN_ADD_FAILED", "Se falló creando el dominio");
define("PQL_LANG_DOMAIN_DEL", "Eliminar dominio");
define("PQL_LANG_DOMAIN_DEL_TITLE", "Eliminar dominio %domain%");
define("PQL_LANG_DOMAIN_DEL_WARNING", "<b>Atencion:</b> Si eliminas un dominio, todos los usuarios dentro de ese odminio también serán eliminados !!");
define("PQL_LANG_DOMAIN_DEL_OK", "Dominio eliminado con éxito");
define("PQL_LANG_DOMAIN_DEL_FAILED", "Se falló al eliminar el dominio");

define("PQL_LANG_ADDRESS_REGISTRED", "Direcciones registradas");
define("PQL_LANG_ADDRESS_TYPE", "Tipo");

define("PQL_LANG_SENDMAIL", "Enviar mail de prueba");
define("PQL_LANG_SENDMAIL_OK", "Mail enviado con éxito a %email%");
define("PQL_LANG_SENDMAIL_FAILED", "Se falló al enviar el mail");

// deliverymode texts
// (normal, forwardonly, nombox, localdelivery, reply, echo)
define("PQL_LANG_DELIVERYMODE_TITLE","Modo de entrega");
define("PQL_LANG_DELIVERYMODE_MODE","Modo");
define("PQL_LANG_DELIVERYMODE_NORMAL","Normal");
define("PQL_LANG_DELIVERYMODE_FORWARDONLY","SOlo forward");
define("PQL_LANG_DELIVERYMODE_NOMBOX","No mailbox local");
define("PQL_LANG_DELIVERYMODE_LOCALDELIVERY","Guardar local");
define("PQL_LANG_DELIVERYMODE_REPLY","Auto reply");
define("PQL_LANG_DELIVERYMODE_REPLY_SEND","Enviar un auto reply");
define("PQL_LANG_DELIVERYMODE_REPLY_TEXT","Texto de contestación");
define("PQL_LANG_DELIVERYMODE_ECHO","Pasar a la consola (tricky)");
define("PQL_LANG_DELIVERYMODE_CHANGE","Cambiar propiedades");
define("PQL_LANG_DELIVERYMODE_CHANGE_OK","Modo de entrega cambiado");
define("PQL_LANG_DELIVERYMODE_CHANGE_FAILED","Se falló al cambiar el modo de entrega");
define("PQL_LANG_DELIVERYMODE_NULL","Propiedades por defecto");
define("PQL_LANG_DELIVERYMODE_PROFILE", "Perfil");
define("PQL_LANG_DELIVERYMODE_PROFILE_LOCAL", "Normal (mailbox local, cuenta POP)");
define("PQL_LANG_DELIVERYMODE_PROFILE_FORWARD", "Forward (solo forward, no mailbox local)");

// accountstatus texts
// (active, nopop, disable)
define("PQL_LANG_ACCOUNTSTATUS_TITLE","Estado de la cuenta");
define("PQL_LANG_ACCOUNTSTATUS_STATUS","Estado");
define("PQL_LANG_ACCOUNTSTATUS_ACTIVE","activo");
define("PQL_LANG_ACCOUNTSTATUS_NOPOP","POP cerrado");
define("PQL_LANG_ACCOUNTSTATUS_DISABLE","cerrada");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE","Modificar estado");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE_OK","Estado cambiado a %status% con éxito");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE_FAILED","Se falló al cambiar el estado");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE_ACTIVE","activar");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE_NOPOP","cerrar la cuenta POP");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE_DISABLE","cerrar");

// mail texts
define("PQL_LANG_MAIL_TITLE","Dirección principal");
define("PQL_LANG_MAIL_CHANGE","Cambiar dirección principal");
define("PQL_LANG_MAIL_HELP", "Una dirección debe ser única en todo el dominio");
define("PQL_LANG_MAIL_CHANGE_OK", "Dirección principal cambiada con exitp de %old% a %new%");
define("PQL_LANG_MAIL_CHANGE_FAILED", "Se falló al cambiar la dirección principal");
define("PQL_LANG_MAIL_UNCHANGED", "No se cambió la dirección principal");

// mailforwardingaddress texts
define("PQL_LANG_MAILFORWARDINGADDRESS_TITLE","Forward");
define("PQL_LANG_MAILFORWARDINGADDRESS_OTHER", "Forwarders de otras cuentas de este usuario");
define("PQL_LANG_MAILFORWARDINGADDRESS_NEW","Crear nueva dirección de forwarding");
define("PQL_LANG_MAILFORWARDINGADDRESS_ADD_HELP", "Puedes añadir más direcciones de forwarding en la página de detalles del usuario");
define("PQL_LANG_MAILFORWARDINGADDRESS_ADD_OK", "Forward a %mail% creado con éxito");
define("PQL_LANG_MAILFORWARDINGADDRESS_ADD_FAILED", "Se falló creando el forward");
define("PQL_LANG_MAILFORWARDINGADDRESS_NONE","ninguno");
define("PQL_LANG_MAILFORWARDINGADDRESS_CHANGE", "Cambiar dirección de forwarding");
define("PQL_LANG_MAILFORWARDINGADDRESS_CHANGE_OK", "Dirección de forwarding cambiada con éxito de %old% a %new%");
define("PQL_LANG_MAILFORWARDINGADDRESS_CHANGE_FAILED", "Se falló al modificar la dirección de forwarding");
define("PQL_LANG_MAILFORWARDINGADDRESS_UNCHANGED", "No se cambió la dirección de forwarding");

// mailalternateaddress texts
define("PQL_LANG_MAILALTERNATEADDRESS_TITLE","Alias");
define("PQL_LANG_MAILALTERNATEADDRESS_NEW","Crear nuevo alias");
define("PQL_LANG_MAILALTERNATEADDRESS_DEL", "Eliminar alias");
define("PQL_LANG_MAILALTERNATEADDRESS_HELP", "Un alias debe ser único en todo el dominio");
define("PQL_LANG_MAILALTERNATEADDRESS_ADD_OK", "Alias %mail% creado con éxito");
define("PQL_LANG_MAILALTERNATEADDRESS_ADD_FAILED", "Se falló al crear el alias");
define("PQL_LANG_MAILALTERNATEADDRESS_CHANGE", "Cambiar alias");
define("PQL_LANG_MAILALTERNATEADDRESS_CHANGE_OK", "Alias cambiado de %old% a %new% con éxito");
define("PQL_LANG_MAILALTERNATEADDRESS_CHANGE_FAILED", "Se falló modificando el alias");
define("PQL_LANG_MAILALTERNATEADDRESS_UNCHANGED","No se cambió el alias");

// userpassword texts
define("PQL_LANG_USERPASSWORD_TITLE", "Contraseña");
define("PQL_LANG_USERPASSWORD_NONE", "Ninguna");
define("PQL_LANG_USERPASSWORD_ENCRYPTED", "encriptada");
define("PQL_LANG_USERPASSWORD_NEW", "Cambiar contraseña");
define("PQL_LANG_USERPASSWORD_RETYPE", "Contraseña (reescribir)");
define("PQL_LANG_USERPASSWORD_HELP", "Una contrasña solo puede contener númerps y letras, los carácteres especiales no están permitidos");
define("PQL_LANG_USERPASSWORD_CHANGE_OK", "Contrasñea cambiada con éxito");
define("PQL_LANG_USERPASSWORD_CHANGE_FAILED", "Se falló al cambiar la contraseña");

// uid texts
define("PQL_LANG_UID_TITLE", "Nombre de usuario");
define("PQL_LANG_UID_NONE", "ninguno");
define("PQL_LANG_UID_HELP", "El nombre de usuario solo puede contener números, letras, y los siguientes carácteres especiales &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;. Adicionalmente, el nombre de usuario debe ser único en todo el sistema.");
define("PQL_LANG_UID_HELP_SHORT", "números, letras, y los siguientes carácteres especiales: &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;");
define("PQL_LANG_UID_CHANGE", "Cambiar nombre de usuario");
define("PQL_LANG_UID_CHANGE_OK", "Nombre de usuario cambiado con éxito de %old% a %new%");
define("PQL_LANG_UID_CHANGE_FAILED", "Se falló al cambiar el nombre de usuario");
define("PQL_LANG_UID_UNCHANGED","No se cambió el nombre de usuario");

// mailquota texts
define("PQL_LANG_MAILQUOTA_TITLE", "Cuota");
define("PQL_LANG_MAILQUOTA_NONE", "estándar");
define("PQL_LANG_MAILQUOTA_ECHO_MAXSIZE", "máximo %maxsize% KB");
define("PQL_LANG_MAILQUOTA_ECHO_MAXSIZE_UNLIMITED", "tamaño ilimitado");
define("PQL_LANG_MAILQUOTA_ECHO_MAXMAILS", "máximo %maxmails% mails");
define("PQL_LANG_MAILQUOTA_ECHO_MAXMAILS_UNLIMITED", "numero ilimitado de mails");
define("PQL_LANG_MAILQUOTA_HELP", "Con 'estándar', qmail/ldap obtendrá las cuotas desde su fichero de ocnfiguración");
define("PQL_LANG_MAILQUOTA_NONESELECTED", "Debes seleccionar al menos un tipo de cuota");
define("PQL_LANG_MAILQUOTA_MAXSIZE_TITLE", "Tamaño máximo del mailbox");
define("PQL_LANG_MAILQUOTA_MAXMAILS_TITLE", "Máximo número de mails");
define("PQL_LANG_MAILQUOTA_DEFAULT", "Estándar");
define("PQL_LANG_MAILQUOTA_USERDEFINED", "Definido por el usuario");
define("PQL_LANG_MAILQUOTA_CHANGE", "Cambiar cuota");
define("PQL_LANG_MAILQUOTA_CHANGE_OK", "Cuota cambiada a %quota% con éxito");
define("PQL_LANG_MAILQUOTA_CHANGE_FAILED", "Se falló al cambiar la cuota");
define("PQL_LANG_MAILQUOTA_UNCHANGED","No se cambió la cuota");

// mailhost texts
define("PQL_LANG_MAILHOST_TITLE", "Servidor de correo");
define("PQL_LANG_MAILHOST_FQDN", "Dirección del host (FQDN)");
define("PQL_LANG_MAILHOST_NONE", "ninguna");
define("PQL_LANG_MAILHOST_CHANGE", "Cambiar la dirección de host del servidor de correo");
define("PQL_LANG_MAILHOST_CHANGE_OK", "Servidor de correo cambiado a %mailhost% con éxito");
define("PQL_LANG_MAILHOST_CHANGE_FAILED", "Se falló al cambiar el servidor de correo");
define("PQL_LANG_MAILHOST_DEFAULT", "Estándar (entrada DNS)");
define("PQL_LANG_MAILHOST_USERDEFINED", "Definido por el usuario");
define("PQL_LANG_MAILHOST_UNCHANGED", "No se cambió el servidor de correo");
define("PQL_LANG_MAILHOST_NOOPTION", "Ninguna opción ha sido seleccionada");
define("PQL_LANG_MAILHOST_HELP", "Con la opción DNS, el primer registro MX de este dominio será tomado");

// mailmessagestore texts
define("PQL_LANG_MAILMESSAGESTORE_TITLE", "Ruta al mailbox");
define("PQL_LANG_MAILMESSAGESTORE_HELP", "Si no proporcionas una ruta absoluta, será prefijada con el valor en qmail/control/ldapmessagestore. '..' no está permitido.");
define("PQL_LANG_MAILMESSAGESTORE_ABSOLUTE_NOTALLOWED", "Las rutas absolutas no son aceptadas por la configuración");
define("PQL_LANG_MAILMESSAGESTORE_NONE", "ninguna");
define("PQL_LANG_MAILMESSAGESTORE_CHANGE", "Cambiar ruta al mailbox");
define("PQL_LANG_MAILMESSAGESTORE_CHANGE_OK", "Ruta al mailboz cambiada a %path% con éxito");
define("PQL_LANG_MAILMESSAGESTORE_CHANGE_FAILED", "Se falló al cambiar la ruta la mailbox");
define("PQL_LANG_MAILMESSAGESTORE_UNCHANGED", "No se cambió la ruta al mailbox");

// qmaildotmode texts
define("PQL_LANG_QMAILDOTMODE_TITLE", "dotmode");
define("PQL_LANG_QMAILDOTMODE_HELP", "Si seleccionas dotmode como ambos o ldapwithprog deberás añadir un deliveryprogram también");
define("PQL_LANG_QMAILDOTMODE_NONE", "por defecto");
define("PQL_LANG_QMAILDOTMODE_CHANGE", "Cambiar dotmode");
define("PQL_LANG_QMAILDOTMODE_CHANGE_OK", "Dotmode cambiado a %mode% con éxito");
define("PQL_LANG_QMAILDOTMODE_CHANGE_FAILED", "Se falló al cambiar el dotmode");
define("PQL_LANG_QMAILDOTMODE_UNCHANGED", "No se cambió el dotmode");
define("PQL_LANG_QMAILDOTMODE_MODE_DEFAULT", "por defecto (tomado de ldapdefaultdotmode)");
define("PQL_LANG_QMAILDOTMODE_MODE_BOTH", "ambos (deliveryProgramPath + .qmail)");
define("PQL_LANG_QMAILDOTMODE_MODE_LDAPONLY", "ldaponly (deliveryProgramPath + .qmail ignorado para entregas por ldap)");
define("PQL_LANG_QMAILDOTMODE_MODE_LDAPWITHPROG", "ldapwithprog (deliveryProgramPath usado, .qmail ignorado)");
define("PQL_LANG_QMAILDOTMODE_MODE_NONE", "ninguno (deliveryProgramPath + .qmail ignorados)");

// deliveryprogrampath texts
define("PQL_LANG_DELIVERYPROGRAMPATH_TITLE", "Deliveryprogram");
define("PQL_LANG_DELIVERYPROGRAMPATH_HELP", "Esto es lo mismo que '|/path/to/someprog' en un archivo .qmail. Solo usado si qmailDotMode es ldapwithprog o ambos.");
define("PQL_LANG_DELIVERYPROGRAMPATH_NONE", "ninguno");
define("PQL_LANG_DELIVERYPROGRAMPATH_CHANGE", "Cambiar la ruta al deliveryprogram");
define("PQL_LANG_DELIVERYPROGRAMPATH_CHANGE_OK", "Ruta cambiada a %path% con éxito");
define("PQL_LANG_DELIVERYPROGRAMPATH_CHANGE_FAILED", "Se falló al cambiar la ruta");
define("PQL_LANG_DELIVERYPROGRAMPATH_UNCHANGED", "No se cambió la ruta");
define("PQL_LANG_DELIVERYPROGRAMPATH_DOTWARNING", "El qmaildotmode de este usario es %qmaildotmode%. Deberás cambiarlo a ambos o ldapwithprog para conseguir que el deliveryprogram se ejecute.");

// search engine texts
define("PQL_LANG_SEARCH_TITLE","Encontrar usuarios por");
define("PQL_LANG_SEARCH_TITLE2","Resultados de la búsqueda");
define("PQL_LANG_SEARCH_UID","Nombre de usuario");
define("PQL_LANG_SEARCH_CN","Nombre común");
define("PQL_LANG_SEARCH_MAIL","Dirección de correo");
define("PQL_LANG_SEARCH_MAILALTERNATEADDRESS","Alias de correo");
define("PQL_LANG_SEARCH_MAILFORWARDINGADDRESS","Dirección de correo Forwarding");
define("PQL_LANG_SEARCH_CONTAINS","Contiene");
define("PQL_LANG_SEARCH_IS","Es");
define("PQL_LANG_SEARCH_STARTSWITH","Empieza con");
define("PQL_LANG_SEARCH_ENDSWITH","Termina con");
define("PQL_LANG_SEARCH_FINDBUTTON","buscar");

// ldap connection test texts
define("PQL_LANG_TEST_TITLE", "conexión ldap de phpQLAdmin");
define("PQL_LANG_TEST_TESTS", "Pruebas");
define("PQL_LANG_TEST_HELP", "Esta es una simple prueba para mostrar si la extensión de ldap está cargada y las conexiones están funcionando. No se realizan pruebas de derechos de acceso al directorio.");
define("PQL_LANG_TEST_LDAP_EXT_TITLE", "extensión ldap de php");
define("PQL_LANG_TEST_LDAP_EXT_NA", "no disponible");
define("PQL_LANG_TEST_LDAP_EXT_OK", "construida (cargada)");
define("PQL_LANG_TEST_CONNECTION_FAILED", "falló");
define("PQL_LANG_TEST_CONNECTION_RESOLVE_ERR", "El nombre de dominio %host% no pudo ser resuelto");
define("PQL_LANG_TEST_CONNECTION_CONNECT_ERR", "No se pudo conectar por el puerto 389 a %host%, por favor asegurese de que el servicio está en funcionamiento y de que no está bloqueado por un cortafuegos");
define("PQL_LANG_TEST_CONNECTION_CONNECT_OK", "ok, disfruta ;-)");
define("PQL_LANG_TEST_CONNECTION_USER_TITLE", "conexión al directorio de usuarios");
define("PQL_LANG_TEST_CONNECTION_USER_BIND_ERR", "la conexión es correcta, pero no se pudo acceder al directorio.");
define("PQL_LANG_TEST_CONNECTION_CONROL_TITLE", "conexión al directorio de control");
define("PQL_LANG_TEST_CONNECTION_CONTROL_BIND_ERR", "la conexión es correcta, pero no se pudo acceder al directorio. Por favor comprueba PQL_CONF_CONTROL_ROOTDN y PQL_CONF_CONTROL_ROOTPW");
define("PQL_LANG_TEST_CONNECTION_CONROL_INACTIVE", "extensión de control desactivada");


// search users by letter
define("PQL_CONF_SEARCH_LETTER_SEARCH", "Buscar por primer letra del nombre de usuario");
define("PQL_CONF_SEARCH_LETTER_ALL","Todos");
define("PQL_CONF_SEARCH_LETTER_NUMBER","Número de usuarios encontrados");
define("PQL_CONF_SEARCH_LETTER_25PREV","25 anteriores");
define("PQL_CONF_SEARCH_LETTER_25NEXT","25 siguientes");

// Address books
define("PQL_LANG_ADDRESS_BOOK_ADITIONAL", "<b>Campos adicionales para la libreta de direcciones de MS Outlook Express</b><br>No se aceptan ni la letra ñ ni acentos");
define("PQL_LANG_ADDRESS_BOOK_NETSCAPE", "* Campo aceptado por la libreta de direcciones de Netscape Messenger");
define("PQL_LANG_ADDRESS_BOOK_HOME", "DOMICILIO");
define("PQL_LANG_ADDRESS_BOOK_HOME_LOWER","domicilio");
define("PQL_LANG_ADDRESS_BOOK_BUSINESS", "NEGOCIOS");
define("PQL_LANG_ADDRESS_BOOK_BUSINESS_LOWER", "negocios");
define("PQL_LANG_ADDRESS_BOOK_OTHER", "OTROS");
define("PQL_LANG_ADDRESS_BOOK_PHONE", "Teléfono");
define("PQL_LANG_ADDRESS_BOOK_MOBILE", "Móvil");
define("PQL_LANG_ADDRESS_BOOK_ORGANIZATION", "Organización");
define("PQL_LANG_ADDRESS_BOOK_POSTALADDRESS", "Dirección postal");
define("PQL_LANG_ADDRESS_BOOK_CITY", "Ciudad");
define("PQL_LANG_ADDRESS_BOOK_STATE", "Estado o provincia");
define("PQL_LANG_ADDRESS_BOOK_POSTALCODE", "Código postal");
define("PQL_LANG_ADDRESS_BOOK_COUNTRY", "País o región");
define("PQL_LANG_ADDRESS_BOOK_TITLE", "Puesto");
define("PQL_LANG_ADDRESS_BOOK_OFFICE", "Oficina");
define("PQL_LANG_ADDRESS_BOOK_BEEPER", "Localizador");
define("PQL_LANG_ADDRESS_BOOK_INFO", "Notas");
define("PQL_LANG_ADDRESS_BOOK_UNCHANGED", "El valor del campo no ha sido cambiado");
define("PQL_LANG_ADDRESS_BOOK_CHANGE_OK", "Atributo cambiado con éxito");
define("PQL_LANG_ADDRESS_BOOK_CHANGE_FAILED", "Se falló al cambiar el atributo");
define("PQL_LANG_ADDRESS_BOOK_CHANGE", "Cambiar ");
?>
