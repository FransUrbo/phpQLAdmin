<?php
// lang.de.inc
// german language definition file
// $Id: lang.de.inc,v 1.2 2002/06/24 14:24:08 blorson Exp $
//
// if you have a translation to another language, please contact
// me --> Matthias Blaser <mb@adfinis.ch>

// language definition
define("PQL_LANG","de");

// welcome texts (home.php)
define("PQL_WELCOME", "Willkommen bei ");
define("PQL_DESCRIPTION","Der QMail-LDAP Adminstration !!");

// general texts
define("PQL_SAVE", "Speichern");
define("PQL_OPTIONS", "Optionen");
define("PQL_EMAIL", "E-Mail");
define("PQL_ACTIONS", "Aktionen");

define("PQL_SURE", "Sind Sie wirklich sicher ??");
define("PQL_YES", "Jaja...");
define("PQL_NO", "Nein");
define("PQL_ADD", "erstellen");

define("PQL_DOCUMENTATION", "Dokumentation");
define("PQL_SHOW_CONFIG", "Konfiguration anzeigen");
define("PQL_TEST_LDAP", "LDAP Verbindung testen");

// error messages
define("PQL_INVALID", "ungültig");
define("PQL_MISSING", "fehlt");
define("PQL_EXISTS", "existiert bereits");
define("PQL_FAILED_RETYPE", "Kontrolle falsch");
define("PQL_NO_OBJECT", "Attribut nicht gefunden");
define("PQL_GREATER_ZERO", "muss > 0 sein");


// user/domain/address things
define("PQL_USER", "Benutzer");
define("PQL_USER_ID", "Benutzername");
define("PQL_USER_DATA", "Benutzerdaten");
define("PQL_USER_DATA_SURNAME", "Vorname");
define("PQL_USER_DATA_NAME", "Name");

define("PQL_USER_REGISTRED","Registrierte Benutzer");
define("PQL_USER_NEW", "Neuen Benutzer registrieren");
define("PQL_USER_DELETE", "Benutzer löschen");
define("PQL_USER_DELETE_FORWARDS", "Lösche auch Weiterleitungen zu diesem Benutzer");
define("PQL_USER_EDIT", "Benutzerdaten verändern");
define("PQL_USER_EXIST", "Der Benutzer %user% existiert bereits");
define("PQL_USER_NONE", "keine Benutzer eingetragen");
define("PQL_USER_MAILBOXPROPERTIES", "Mailbox Einstellungen");
define("PQL_USER_ADD_TITLE", "Benutzer für %domain% eintragen");
define("PQL_USER_ADD_HELP1", "Die Kombination &quot;Vorname Nachname&quot; (z.b Hans Muster) muss innerhalb der Domain eindeutig sein. Der Benutzername muss innerhalb des gesamten Systems eindeutig sein.");
define("PQL_USER_ADD_HELP2", "Die Emailadresse und der Benutzername werden automatisch in Kleinbuchstaben umgewandelt.");
define("PQL_USER_ADD_OK", "Benutzer erfolgreich erstellt");
define("PQL_USER_ADD_FAILED", "Fehler beim erstellen des Benutzers");
define("PQL_USER_ACCOUNT_PROPERTIES", "Accounteinstellungen");
define("PQL_USER_ACCOUNT_PROPERTIES_MORE", "Weitere Accounteinstellungen");
define("PQL_USER_ACCOUNT_TYPE", "Typ");
define("PQL_USER_DEL_TITLE", "%user% aus %domain% entfernen");
define("PQL_USER_DEL_OK", "Benutzer erfolgreich entfernt");
define("PQL_USER_DEL_FAILED", "Fehler beim entfernen des Benutzers");
define("PQL_USER_DEL_ATTRIBUTE_TITLE", "Attribut (%value%) entfernen");
define("PQL_USER_DEL_ATTRIBUTE_OK", "Attribut entfernt");
define("PQL_USER_DEL_ATTRIBUTE_FAILED", "Fehler beim entfernen des Attributs");
define("PQL_USER_DELIVERYPROPERTIES","Erweiterte Verbindungseinstellungen");

define("PQL_DNS_TITLE", "DNS Informationen");
define("PQL_DNS_MAILHOST", "Mailhost");
define("PQL_DNS_PRIORITY", "Priorität");
define("PQL_DNS_NONE", "Domain ist nicht im DNS eingetragen !!");

define("PQL_DOMAIN_CHANGE_ATTRIBUTE_TITLE", "Werte aller Benutzer der Domain ändern");
define("PQL_DOMAIN_CHANGE_ATTRIBUTE", "Attribut");
define("PQL_DOMAIN_INVALID", "Ungültiger Domain-Name !! Format: domain.tld (z.B adfinis.com)");
define("PQL_DOMAIN_EXISTS", "Diese Domain ist bereits vorhanden");
define("PQL_DOMAIN_ADD", "Domain erstellen");
define("PQL_DOMAIN_ADD_OK", "Die Domain %domain% wurde erstellt. <br> Die Domain muss zusätzlich in der Datei locals und rcpthosts im Control-Verzeichnis von Qmail eingetragen werden !!");
define("PQL_DOMAIN_ADD_FAILED", "Fehler beim Erstellen der Domain");
define("PQL_DOMAIN_DEL", "Domain entfernen");
define("PQL_DOMAIN_DEL_TITLE", "Domain %domain% entfernen");
define("PQL_DOMAIN_DEL_FORWARDS", "Lösche auch alle Weiterleitungen, die zu Benutzern dieser Domain zeigen");
define("PQL_DOMAIN_DEL_WARNING", "<b>Achtung:</b> Beim Entfernen der Domain werden alle eingetragenen Benutzer gelöscht !!");
define("PQL_DOMAIN_DEL_OK", "Domain erfolgreich entfernt");
define("PQL_DOMAIN_DEL_FAILED", "Fehler beim entfernen der Domain");

define("PQL_ADDRESS_REGISTRED", "Registrierte Adressen");
define("PQL_ADDRESS_TYPE", "Typ");

define("PQL_SENDMAIL", "Testmail versenden");
define("PQL_SENDMAIL_OK", "Mail erfolgreich an %email% gesendet");
define("PQL_SENDMAIL_FAILED", "Fehler beim versenden des Mails");

// deliverymode texts
// (normal, forwardonly, nombox, localdelivery, reply, echo)
define("PQL_LDAP_DELIVERYMODE_TITLE","Übertragungseinstellungen");
define("PQL_LDAP_DELIVERYMODE_MODE","Modus");
define("PQL_LDAP_DELIVERYMODE_NORMAL","Normal");
define("PQL_LDAP_DELIVERYMODE_FORWARDONLY","Nur weiterleiten");
define("PQL_LDAP_DELIVERYMODE_NOMBOX","Keine lokale Mailbox");
define("PQL_LDAP_DELIVERYMODE_LOCALDELIVERY","Lokal speichern");
define("PQL_LDAP_DELIVERYMODE_REPLY","Automatische Antwort");
define("PQL_LDAP_DELIVERYMODE_REPLY_SEND","Automatische Antwort versenden");
define("PQL_LDAP_DELIVERYMODE_REPLY_TEXT","Antworttext");
define("PQL_LDAP_DELIVERYMODE_ECHO","Ausgabe an Konsole (tricky)");
define("PQL_LDAP_DELIVERYMODE_CHANGE","Einstellungen ändern");
define("PQL_LDAP_DELIVERYMODE_CHANGE_OK","Übertragungseinstellungen geändert");
define("PQL_LDAP_DELIVERYMODE_CHANGE_FAILED","Änderung der Übertragungseinstellungen fehlgeschlagen");
define("PQL_LDAP_DELIVERYMODE_NULL","Standardeinstellungen");
define("PQL_LDAP_DELIVERYMODE_PROFILE", "Profil");
define("PQL_LDAP_DELIVERYMODE_PROFILE_LOCAL", "Normal (Lokale Mailbox, POP-Account)");
define("PQL_LDAP_DELIVERYMODE_PROFILE_FORWARD", "Forward (Nur weiterleiten, keine lokale Mailbox)");

// accountstatus texts
// (active, nopop, disable)
define("PQL_LDAP_ACCOUNTSTATUS_TITLE","Status des Mailaccounts");
define("PQL_LDAP_ACCOUNTSTATUS_STATUS","Status");
define("PQL_LDAP_ACCOUNTSTATUS_ACTIVE","aktiv");
define("PQL_LDAP_ACCOUNTSTATUS_NOPOP","POP gesperrt");
define("PQL_LDAP_ACCOUNTSTATUS_DISABLE","gesperrt");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE","Status setzen");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_OK","Status auf %status% gesetzt");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_FAILED","Fehler beim Setzen des Status");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_ACTIVE","aktivieren");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_NOPOP","POP sperren");
define("PQL_LDAP_ACCOUNTSTATUS_CHANGE_DISABLE","sperren");

// mail texts
define("PQL_LDAP_MAIL_TITLE","Hauptadresse");
define("PQL_LDAP_MAIL_CHANGE","Hauptadresse ändern");
define("PQL_LDAP_MAIL_HELP", "Eine Email-Adresse muss innerhalb der gesamten Domain eindeutig sein.");
define("PQL_LDAP_MAIL_CHANGE_OK", "Hauptadresse von %old% nach %new% geändert");
define("PQL_LDAP_MAIL_CHANGE_FAILED", "Veränderung der Hauptadresse fehlgeschlagen");
define("PQL_LDAP_MAIL_UNCHANGED", "Hauptadresse unverändert");

// mailforwardingaddress texts
define("PQL_LDAP_MAILFORWARDINGADDRESS_TITLE","Weiterleitung");
define("PQL_LDAP_MAILFORWARDINGADDRESS_OTHER", "Weiterleitungen anderer Accounts zu diesem Benutzer");
define("PQL_LDAP_MAILFORWARDINGADDRESS_NEW","Neue Weiterleitung hinzufügen");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_HELP", "Weitere Weiterleitungen können auf der Detailseite eines Benutzers eintragen werden");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_OK", "Weiterleitung nach %mail% erstellt");
define("PQL_LDAP_MAILFORWARDINGADDRESS_ADD_FAILED", "Fehler beim Erstellen der Weiterleitung");
define("PQL_LDAP_MAILFORWARDINGADDRESS_NONE","keine");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE", "Weiterleitung ändern");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE_OK", "Weiterleitung von %old% nach %new% geändert");
define("PQL_LDAP_MAILFORWARDINGADDRESS_CHANGE_FAILED", "Veränderung der Weiterleitung fehlgeschlagen");
define("PQL_LDAP_MAILFORWARDINGADDRESS_UNCHANGED", "Weiterleitung unverändert");

// mailalternateaddress texts
define("PQL_LDAP_MAILALTERNATEADDRESS_TITLE","Alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_NEW","Neues Alias hinzufügen");
define("PQL_LDAP_MAILALTERNATEADDRESS_DEL", "Alias löschen");
define("PQL_LDAP_MAILALTERNATEADDRESS_DEL_FORWARDS", "Lösche auch alle Weiterleitungen zu diesem Alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_HELP", "Eine Email-Adresse (auch ein Alias) muss innerhalb der gesamten Domain eindeutig sein.");
define("PQL_LDAP_MAILALTERNATEADDRESS_ADD_OK", "Alias %mail% erstellt");
define("PQL_LDAP_MAILALTERNATEADDRESS_ADD_FAILED", "Fehler beim Erstellen des Alias");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE", "Alias ändern");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE_OK", "Alias von %old% nach %new% geändert");
define("PQL_LDAP_MAILALTERNATEADDRESS_CHANGE_FAILED", "Veränderung des Alias fehlgeschlagen");
define("PQL_LDAP_MAILALTERNATEADDRESS_UNCHANGED","Alias unverändert");

// userpassword texts
define("PQL_LDAP_USERPASSWORD_TITLE", "Passwort");
define("PQL_LDAP_USERPASSWORD_NONE", "kein");
define("PQL_LDAP_USERPASSWORD_ENCRYPTED", "verschlüsselt");
define("PQL_LDAP_USERPASSWORD_NEW", "Passwort neu setzen");
define("PQL_LDAP_USERPASSWORD_RETYPE", "Passwort (Kontrolle)");
define("PQL_LDAP_USERPASSWORD_HELP", "Ein Passwort darf nur aus Ziffern und Buchstaben bestehen, es sind keine Sonderzeichen erlaubt");
define("PQL_LDAP_USERPASSWORD_CHANGE_OK", "Passwort wurde geändert");
define("PQL_LDAP_USERPASSWORD_CHANGE_FAILED", "Fehler beim Ändern des Passworts");

// uid texts
define("PQL_LDAP_UID_TITLE", "Benutzername");
define("PQL_LDAP_UID_NONE", "kein");
define("PQL_LDAP_UID_HELP", "Der Benutzername darf nur aus Ziffern, Buchstaben und den Sonderzeichen &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot; bestehen. Zudem muss der Benutzername innerhalb des gesamten Systems eindeutig sein.");
define("PQL_LDAP_UID_HELP_SHORT", "Ziffern, Buchstaben und Sonderzeichen &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;");
define("PQL_LDAP_UID_CHANGE", "Benutzername verändern");
define("PQL_LDAP_UID_CHANGE_OK", "Benutzername von %old% nach %new% geändert");
define("PQL_LDAP_UID_CHANGE_FAILED", "Fehler beim Ändern des Benutzernamens");
define("PQL_LDAP_UID_UNCHANGED","Benutzername unverändert");

// mailquota texts
define("PQL_LDAP_MAILQUOTA_TITLE", "Beschränkungen");
define("PQL_LDAP_MAILQUOTA_NONE", "Standard");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE", "max %maxsize% KB");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXSIZE_UNLIMITED", "unlimitierte Grösse");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS", "max %maxmails% Mails");
define("PQL_LDAP_MAILQUOTA_ECHO_MAXMAILS_UNLIMITED", "unlimitierte Anzahl Mails");
define("PQL_LDAP_MAILQUOTA_HELP", "Bei Standard werden die Standardeinstellungen von qmail/LDAP übernommen.");
define("PQL_LDAP_MAILQUOTA_NONESELECTED", "Bei Benutzdefiniert muss mindestens eine Beschränkung aktiviert werden");
define("PQL_LDAP_MAILQUOTA_MAXSIZE_TITLE", "Max. Grösse der Mailbox");
define("PQL_LDAP_MAILQUOTA_MAXMAILS_TITLE", "Max. Anzahl Mails");
define("PQL_LDAP_MAILQUOTA_DEFAULT", "Standard");
define("PQL_LDAP_MAILQUOTA_USERDEFINED", "Benutzerdefiniert");
define("PQL_LDAP_MAILQUOTA_CHANGE", "Beschränkungen verändern");
define("PQL_LDAP_MAILQUOTA_CHANGE_OK", "Beschränkung auf %quota% gesetzt");
define("PQL_LDAP_MAILQUOTA_CHANGE_FAILED", "Fehler beim Ändern der Beschränkung");
define("PQL_LDAP_MAILQUOTA_UNCHANGED","Beschränkung unverändert");

// mailhost texts
define("PQL_LDAP_MAILHOST_TITLE", "Mailserver");
define("PQL_LDAP_MAILHOST_FQDN", "Adresse (FQDN)");
define("PQL_LDAP_MAILHOST_NONE", "kein");
define("PQL_LDAP_MAILHOST_CHANGE", "Mailserver ändern");
define("PQL_LDAP_MAILHOST_CHANGE_OK", "Mailserver auf %mailhost% geändert");
define("PQL_LDAP_MAILHOST_CHANGE_FAILED", "Fehler beim Ändern des Mailservers");
define("PQL_LDAP_MAILHOST_DEFAULT", "Standard (DNS-Eintrag)");
define("PQL_LDAP_MAILHOST_USERDEFINED", "Benutzerdefiniert");
define("PQL_LDAP_MAILHOST_UNCHANGED", "Mailserver unverändert");
define("PQL_LDAP_MAILHOST_NOOPTION", "Keine Option gesetzt");
define("PQL_LDAP_MAILHOST_HELP", "Bei der Option DNS wird der erste MX-Eintrag für diese Domain übernommen");

// mailmessagestore texts
define("PQL_LDAP_MAILMESSAGESTORE_TITLE", "Mailbox Pfad");
define("PQL_LDAP_MAILMESSAGESTORE_HELP", "Falls der angegebene Pfad nicht absolut angegeben wird, wird es die Einstellungqmail/control/ldapmessagestore vorangestellt. '..' darf nicht verwendet werden.");
define("PQL_LDAP_MAILMESSAGESTORE_ABSOLUTE_NOTALLOWED", "Absolute Pfade wurden in der Konfiguration abgeschaltet.");
define("PQL_LDAP_MAILMESSAGESTORE_NONE", "kein");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE", "Mailbox Pfad ändern");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE_OK", "Mailbox Pfad nach %path% gesetzt");
define("PQL_LDAP_MAILMESSAGESTORE_CHANGE_FAILED", "Ändern des Mailbox Pfades fehlgeschlagen");
define("PQL_LDAP_MAILMESSAGESTORE_UNCHANGED", "Mailbox Pfad unverändert");

// qmaildotmode texts
define("PQL_LDAP_QMAILDOTMODE_TITLE", "dotmode");
define("PQL_LDAP_QMAILDOTMODE_HELP", "Wenn both oder ldapwithprog gewählt wird, sollte auch ein deliveryprogram gesetzt sein.");
define("PQL_LDAP_QMAILDOTMODE_NONE", "standard");
define("PQL_LDAP_QMAILDOTMODE_CHANGE", "dotmode verändern");
define("PQL_LDAP_QMAILDOTMODE_CHANGE_OK", "dotmode auf %mode% gesetzt");
define("PQL_LDAP_QMAILDOTMODE_CHANGE_FAILED", "Ändern des dotmodes fehlgeschlagen");
define("PQL_LDAP_QMAILDOTMODE_UNCHANGED", "dotmode unverändert");
define("PQL_LDAP_QMAILDOTMODE_MODE_DEFAULT", "standard (wird von ldapdefaultdotmode übernommen)");
define("PQL_LDAP_QMAILDOTMODE_MODE_BOTH", "both (deliveryProgramPath + .qmail)");
define("PQL_LDAP_QMAILDOTMODE_MODE_LDAPONLY", "ldaponly (deliveryProgramPath + .qmail für ldap auslieferungen ignoriert)");
define("PQL_LDAP_QMAILDOTMODE_MODE_LDAPWITHPROG", "ldapwithprog (deliveryProgramPath wird benutzt, .qmail ignoriert)");
define("PQL_LDAP_QMAILDOTMODE_MODE_NONE", "none (deliveryProgramPath + .qmail ignoriert)");

// deliveryprogrampath texts
define("PQL_LDAP_DELIVERYPROGRAMPATH_TITLE", "Programm bei Auslieferung (Deliveryprogram)");
define("PQL_LDAP_DELIVERYPROGRAMPATH_HELP", "Dasselbe wie '|/path/to/someprog' in einer .qmail Datei. Wird nur benutzt, falls dotmode auf ldapwithprog oder both gesetzt ist.");
define("PQL_LDAP_DELIVERYPROGRAMPATH_NONE", "kein");
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE", "Programmpfad ändern");
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_OK", "Programmpfad auf %path% gesetzt");
define("PQL_LDAP_DELIVERYPROGRAMPATH_CHANGE_FAILED", "Ändern des Programmpfades fehlgeschlagen");
define("PQL_LDAP_DELIVERYPROGRAMPATH_UNCHANGED", "Programmpfad unverändert");
define("PQL_LDAP_DELIVERYPROGRAMPATH_DOTWARNING", "Der qmaildotmode dieses Benutzers ist auf %qmaildotmode% gesetzt. Damit das Auslieferungsprogramm (Deliveryprogram) ausgeführt wird, setzen Sie qmaildotmode auf both oder ldapwithprog");

// search engine texts
define("PQL_SEARCH_TITLE","Benutzer suchen, deren...");
define("PQL_SEARCH_TITLE2","Suchergebnisse");
define("PQL_SEARCH_UID","Benutzername");
define("PQL_SEARCH_CN","Common Name");
define("PQL_SEARCH_MAIL","Mail Adresse");
define("PQL_SEARCH_MAILALTERNATEADDRESS","Mail Alias");
define("PQL_SEARCH_MAILFORWARDINGADDRESS","Weiterleitungsadresse");
define("PQL_SEARCH_CONTAINS"," folgendes enthält");
define("PQL_SEARCH_IS"," folgendes ist");
define("PQL_SEARCH_STARTSWITH"," mit folgendem beginnt");
define("PQL_SEARCH_ENDSWITH"," mit folgendem aufhört");
define("PQL_SEARCH_FINDBUTTON","Suchen");

// ldap connection test texts
define("PQL_TEST_TITLE", "phpQLAdmin LDAP Verbindung");
define("PQL_TEST_TESTS", "Tests");
define("PQL_TEST_HELP", "Dies ist nur ein einfacher Test ob die nötigen PHP-Erweiterungen für LDAP geladen sind und ob der LDAP-Server erreichbar ist. Es werden keine Tests durchgeführt, die die Rechte im LDAP-Verzeichnis überprüfen");
define("PQL_TEST_LDAP_EXT_TITLE", "PHP LDAP Erweiterung");
define("PQL_TEST_LDAP_EXT_NA", "nicht verfügbar");
define("PQL_TEST_LDAP_EXT_OK", "geladen");
define("PQL_TEST_CONNECTION_FAILED", "fehlgeschlagen");
define("PQL_TEST_CONNECTION_RESOLVE_ERR", "der Hostname %host% konnte nicht aufgelöst werden");
define("PQL_TEST_CONNECTION_CONNECT_ERR", "die Verbindung auf Port 389 auf %host% konnte nicht gestartet werden, überprüfen Sie ob der LDAP-Service auf dem Server gestartet ist und die Verbindung nicht durch eine Firewall blockiert ist");
define("PQL_TEST_CONNECTION_CONNECT_OK", "ok, viel Spass ;-)");
define("PQL_TEST_CONNECTION_USER_TITLE", "Benutzerdatenbank");
define("PQL_TEST_CONNECTION_USER_BIND_ERR", "die Verbindung ist OK, es konnte jedoch nicht mit dem vorhanden Benutzername/Passwort an das Verzeichnis gebunden werden. Bitte überprüfen Sie PQL_LDAP_ROOTDN und PQL_LDAP_ROOTPW");
define("PQL_TEST_CONNECTION_CONROL_TITLE", "Controldatenbank");
define("PQL_TEST_CONNECTION_CONTROL_BIND_ERR", "die Verbindung ist OK, es konnte jedoch nicht mit dem vorhanden Benutzername/Passwort an das Verzeichnis gebunden werden. Bitte überprüfen Sie PQL_LDAP_CONTROL_ROOTDN und PQL_LDAP_CONTROL_ROOTPW");
define("PQL_TEST_CONNECTION_CONROL_INACTIVE", "Servercontrol deaktiviert");
?>