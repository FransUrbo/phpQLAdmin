<?php
// lang.fr.inc
// French language definition file
// $Id: lang.fr.inc,v 1.1 2002/01/04 10:05:01 mb Exp $
//
// french language definition file
// by Julien Andrieux <chill@adx.org>
//
// if you have a translation to another language, please contact
// me --> Turbo Fredriksson <phpqladmin@bayour.com>

// language definition
define("PQL_LANG_LANG","fr");

// welcome texts (home.php)
define("PQL_LANG_WELCOME", "Bienvenue sur ");
define("PQL_LANG_DESCRIPTION","L'outil d'administration de QMail-LDAP !!");

// general texts
define("PQL_LANG_SAVE", "Sauver");
define("PQL_LANG_OPTIONS", "Options");
define("PQL_LANG_EMAIL", "Email");
define("PQL_LANG_ACTIONS", "Actions");

define("PQL_LANG_SURE", "Etes-vous vraiment sûr ??");
define("PQL_LANG_YES", "Ouais...");
define("PQL_LANG_NO", "Non");
define("PQL_LANG_ADD", "Créer");

define("PQL_LANG_DOCUMENTATION", "Documentation");

// error messages
define("PQL_LANG_INVALID", "invalide");
define("PQL_LANG_MISSING", "manquant");
define("PQL_LANG_EXISTS", "déjà existant");
define("PQL_LANG_FAILED_RETYPE", "pas retapé exactement");
define("PQL_LANG_NO_OBJECT", "attribut non trouvé");
define("PQL_LANG_GREATER_ZERO", "doit être > 0");


// user/domain/address things
define("PQL_LANG_USER", "Utilisateur");
define("PQL_LANG_USER_ID", "Identifiant");
define("PQL_LANG_USER_DATA", "Données utilisateur");
define("PQL_LANG_USER_DATA_SURNAME", "Nom");
define("PQL_LANG_USER_DATA_NAME", "Prénom");

define("PQL_LANG_USER_REGISTRED","Utilisateur enregistré");
define("PQL_LANG_USER_NEW", "Enregistrer un nouvel utilisateur");
define("PQL_LANG_USER_DELETE", "Supprimer l'utilisateur");
define("PQL_LANG_USER_EDIT", "Modifier les données de l'utilisateur");
define("PQL_LANG_USER_EXIST", "L'utilisateur %user% existe déjà");
define("PQL_LANG_USER_NONE", "Pas d'utilisateur enregistré");
define("PQL_LANG_USER_MAILBOXPROPERTIES", "Propriétés de la boite email");
define("PQL_LANG_USER_ADD_TITLE", "Créer un utilisateur pour %domain%");
define("PQL_LANG_USER_ADD_HELP1", "La combinaison du prénom et du nom (ex: Zaphod Beeblebrox) doit être unique pour tout le domaine. L'identifiant doit être unique pour l'ensemble du système.");
define("PQL_LANG_USER_ADD_HELP2", "L'adresse email et l'identifiant seront automatiquement convertis en minuscules");
define("PQL_LANG_USER_ADD_OK", "Nouvel utilisateur créé avec succès");
define("PQL_LANG_USER_ADD_FAILED", "Echec dans la création du nouvel utilisateur");
define("PQL_LANG_USER_ACCOUNT_PROPERTIES", "Propriétés du compte");
define("PQL_LANG_USER_ACCOUNT_PROPERTIES_MORE", "Propriétés complémentaires du compte");
define("PQL_LANG_USER_ACCOUNT_TYPE", "Type");
define("PQL_LANG_USER_DEL_TITLE", "Supprimer %user% de %domain%");
define("PQL_LANG_USER_DEL_OK", "Utilisateur supprimé avec succès");
define("PQL_LANG_USER_DEL_FAILED", "Echec dans la supression de l'utilisateur");
define("PQL_LANG_USER_DEL_ATTRIBUTE_TITLE", "Supprimer l'attribut (%value%)");
define("PQL_LANG_USER_DEL_ATTRIBUTE_OK", "Attribut supprimé");
define("PQL_LANG_USER_DEL_ATTRIBUTE_FAILED", "Echec de suppression de l'attribut");

define("PQL_LANG_DNS_TITLE", "Informations DNS");
define("PQL_LANG_DNS_MAILHOST", "Server de mail");
define("PQL_LANG_DNS_PRIORITY", "Priorité");
define("PQL_LANG_DNS_NONE", "Ce domaine n'est pas enregistré dans un DNS");

define("PQL_LANG_DOMAIN_CHANGE_ATTRIBUTE_TITLE", "Changer les valeurs de tous les utilisateurs de ce domaine");
define("PQL_LANG_DOMAIN_CHANGE_ATTRIBUTE", "Attribut");
define("PQL_LANG_DOMAIN_INVALID", "Nom de domaine invalide !! Utilisez: domaine.extension (ex: adfinis.com)");
define("PQL_LANG_DOMAIN_EXISTS", "Ce domaine existe déjà");
define("PQL_LANG_DOMAIN_ADD", "Créer le domaine");
define("PQL_LANG_DOMAIN_ADD_OK", "Domaine %domain% créé avec succès. <br> Vous devez ajouter ce domaine manuellement dans les fichiers rpcthosts et locals du dossier qmail/control !!");
define("PQL_LANG_DOMAIN_ADD_FAILED", "Echec dans la création du domaine");
define("PQL_LANG_DOMAIN_DEL", "Supprimer le domaine");
define("PQL_LANG_DOMAIN_DEL_TITLE", "Supprimer le domaine %domain%");
define("PQL_LANG_DOMAIN_DEL_WARNING", "<b>Attention :</b> Si vous supprimez un domaine, tous les utilisateurs de ce domaine seront supprimés !!");
define("PQL_LANG_DOMAIN_DEL_OK", "Domaine supprimé avec succès");
define("PQL_LANG_DOMAIN_DEL_FAILED", "Echec de suppression du domaine");

define("PQL_LANG_ADDRESS_REGISTRED", "Adresses enregistrées");
define("PQL_LANG_ADDRESS_TYPE", "Type");

define("PQL_LANG_SENDMAIL", "Envoyer un email de test");
define("PQL_LANG_SENDMAIL_OK", "Email à %email% envoyé avec succès");
define("PQL_LANG_SENDMAIL_FAILED", "Echec dans l'envoi de l'email");

// deliverymode texts
// (normal, forwardonly, nombox, localdelivery, reply, echo)
define("PQL_LANG_DELIVERYMODE_TITLE","Mode de distribution");
define("PQL_LANG_DELIVERYMODE_MODE","Mode");
define("PQL_LANG_DELIVERYMODE_NORMAL","Normal");
define("PQL_LANG_DELIVERYMODE_FORWARDONLY","Renvoi uniquement");
define("PQL_LANG_DELIVERYMODE_NOMBOX","Pas de boite email locale");
define("PQL_LANG_DELIVERYMODE_LOCALDELIVERY","Sauvegarder en local");
define("PQL_LANG_DELIVERYMODE_REPLY","Réponse automatique");
define("PQL_LANG_DELIVERYMODE_REPLY_SEND","Envoyer une réponse automatique");
define("PQL_LANG_DELIVERYMODE_REPLY_TEXT","Texte pour la réponse");
define("PQL_LANG_DELIVERYMODE_ECHO","Affichage sur la console (difficile)");
define("PQL_LANG_DELIVERYMODE_CHANGE","Modifier les propriétés");
define("PQL_LANG_DELIVERYMODE_CHANGE_OK","Mode de distribution modifié avec succès");
define("PQL_LANG_DELIVERYMODE_CHANGE_FAILED","Echec dans la modification du mode de distribution");
define("PQL_LANG_DELIVERYMODE_NULL","Propriétés par défaut");
define("PQL_LANG_DELIVERYMODE_PROFILE", "Profil");
define("PQL_LANG_DELIVERYMODE_PROFILE_LOCAL", "Normal (boite aux lettres locale, compte POP)");
define("PQL_LANG_DELIVERYMODE_PROFILE_FORWARD", "Renvoi (renvoi uniquement, pas de boite aux lettres locale)");

// accountstatus texts
// (active, nopop, disable)
define("PQL_LANG_ACCOUNTSTATUS_TITLE","Status du compte");
define("PQL_LANG_ACCOUNTSTATUS_STATUS","Status");
define("PQL_LANG_ACCOUNTSTATUS_ACTIVE","actif");
define("PQL_LANG_ACCOUNTSTATUS_NOPOP","POP verrouillé");
define("PQL_LANG_ACCOUNTSTATUS_DISABLE","verrouillé");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE","Modifier le status");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE_OK","Status modifié pour %status% avec succès");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE_FAILED","Echec dans la modification du status");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE_ACTIVE","activer");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE_NOPOP","verrouiller le compte POP");
define("PQL_LANG_ACCOUNTSTATUS_CHANGE_DISABLE","verrouiller");

// mail texts
define("PQL_LANG_MAIL_TITLE","Adresse principale");
define("PQL_LANG_MAIL_CHANGE","Modifier l'adresse principale");
define("PQL_LANG_MAIL_HELP", "Une adresse doit être unique dans l'ensemble du domaine");
define("PQL_LANG_MAIL_CHANGE_OK", "Adresse principale modifiée avec succès, de %old% à %new%");
define("PQL_LANG_MAIL_CHANGE_FAILED", "Echec dans la modification de l'adresse principale");
define("PQL_LANG_MAIL_UNCHANGED", "Adresse principale inchangée");

// mailforwardingaddress texts
define("PQL_LANG_MAILFORWARDINGADDRESS_TITLE","Renvoi");
define("PQL_LANG_MAILFORWARDINGADDRESS_OTHER", "Renvois vers cet utilisateurs");
define("PQL_LANG_MAILFORWARDINGADDRESS_NEW","Créer une nouvelle adresse de renvoi");
define("PQL_LANG_MAILFORWARDINGADDRESS_ADD_HELP", "Vous pouvez ajouter d'autres adresses de renvoi sur la page de détails d'un utilisateur");
define("PQL_LANG_MAILFORWARDINGADDRESS_ADD_OK", "Renvoi vers %mail% créé avec succès");
define("PQL_LANG_MAILFORWARDINGADDRESS_ADD_FAILED", "Echec dans la création d'un renvoi");
define("PQL_LANG_MAILFORWARDINGADDRESS_NONE","Aucun");
define("PQL_LANG_MAILFORWARDINGADDRESS_CHANGE", "Modifier l'adresse de renvoi");
define("PQL_LANG_MAILFORWARDINGADDRESS_CHANGE_OK", "Adresse de renvoi modifiée avec succès, de %old% à %new%");
define("PQL_LANG_MAILFORWARDINGADDRESS_CHANGE_FAILED", "Echec dans la modification de l'adresse de renvoi");
define("PQL_LANG_MAILFORWARDINGADDRESS_UNCHANGED", "Adresse de renvoi inchangée");

// mailalternateaddress texts
define("PQL_LANG_MAILALTERNATEADDRESS_TITLE","Alias");
define("PQL_LANG_MAILALTERNATEADDRESS_NEW","Créer un nouvel alias");
define("PQL_LANG_MAILALTERNATEADDRESS_DEL", "Supprimer un alias");
define("PQL_LANG_MAILALTERNATEADDRESS_HELP", "un alias doit être unique dans l'ensemble du domaine");
define("PQL_LANG_MAILALTERNATEADDRESS_ADD_OK", "Alias %mail% créé avec succès");
define("PQL_LANG_MAILALTERNATEADDRESS_ADD_FAILED", "Echec dans la création de l'alias");
define("PQL_LANG_MAILALTERNATEADDRESS_CHANGE", "Modifier l'alias");
define("PQL_LANG_MAILALTERNATEADDRESS_CHANGE_OK", "Alias modifié avec succès, de %old% à %new%");
define("PQL_LANG_MAILALTERNATEADDRESS_CHANGE_FAILED", "Echec dans la modification de l'alias");
define("PQL_LANG_MAILALTERNATEADDRESS_UNCHANGED","Alias inchangé");

// userpassword texts
define("PQL_LANG_USERPASSWORD_TITLE", "Mot de passe");
define("PQL_LANG_USERPASSWORD_NONE", "aucun");
define("PQL_LANG_USERPASSWORD_ENCRYPTED", "encrypté");
define("PQL_LANG_USERPASSWORD_NEW", "Modifier password");
define("PQL_LANG_USERPASSWORD_RETYPE", "Mot de passe (confirmation)");
define("PQL_LANG_USERPASSWORD_HELP", "Un mot de passe ne peut contenir que des chiffres et des lettres, les caractères spéciaux ne sont pas autorisés");
define("PQL_LANG_USERPASSWORD_CHANGE_OK", "Mot de passe modifié avec succès");
define("PQL_LANG_USERPASSWORD_CHANGE_FAILED", "Echec dans la modification de mot de passe");

// uid texts
define("PQL_LANG_UID_TITLE", "Identifiant");
define("PQL_LANG_UID_NONE", "aucun");
define("PQL_LANG_UID_HELP", "L'identifiant ne peut contenir que des chiffres, des lettres, et les caractères spéciaux suivant &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;. De plus, l'identifiant doit être unique pour l'ensemble du système.");
define("PQL_LANG_UID_HELP_SHORT", "chiffres, lettres, et les caractères spéciaux suivants: &quot;@&quot;, &quot;.&quot;, &quot;_&quot;, &quot;-&quot;");
define("PQL_LANG_UID_CHANGE", "Modifier l'identifiant");
define("PQL_LANG_UID_CHANGE_OK", "Identifiant modifié avec succès, de %old% à %new%");
define("PQL_LANG_UID_CHANGE_FAILED", "Echec dans la modification de l'identifiant");
define("PQL_LANG_UID_UNCHANGED","Identifiant inchangé");

// mailquota texts
define("PQL_LANG_MAILQUOTA_TITLE", "Quota");
define("PQL_LANG_MAILQUOTA_NONE", "Standard");
define("PQL_LANG_MAILQUOTA_ECHO_MAXSIZE", "maximum %maxsize% KB");
define("PQL_LANG_MAILQUOTA_ECHO_MAXSIZE_UNLIMITED", "Taille illimitée");
define("PQL_LANG_MAILQUOTA_ECHO_MAXMAILS", "maximum %maxmails% messages");
define("PQL_LANG_MAILQUOTA_ECHO_MAXMAILS_UNLIMITED", "nombre illimité de messages");
define("PQL_LANG_MAILQUOTA_HELP", "Avec 'Standard', qmail/ldap recuperera les quotas depuis ses fichiers de configuration");
define("PQL_LANG_MAILQUOTA_NONESELECTED", "Vous devez choisir au moins un type de quota");
define("PQL_LANG_MAILQUOTA_MAXSIZE_TITLE", "Taille maximum de la boite aux lettres");
define("PQL_LANG_MAILQUOTA_MAXMAILS_TITLE", "Nombre maximum de messages");
define("PQL_LANG_MAILQUOTA_DEFAULT", "Standard");
define("PQL_LANG_MAILQUOTA_USERDEFINED", "Défini par l'utilisateur");
define("PQL_LANG_MAILQUOTA_CHANGE", "Modifier les quotas");
define("PQL_LANG_MAILQUOTA_CHANGE_OK", "Quota modifié avec succès pour %quota%");
define("PQL_LANG_MAILQUOTA_CHANGE_FAILED", "Echec dans la modification du quota");
define("PQL_LANG_MAILQUOTA_UNCHANGED","Quota inchangé");

// mailhost texts
define("PQL_LANG_MAILHOST_TITLE", "Serveur de mails");
define("PQL_LANG_MAILHOST_FQDN", "Adresse du serveur (nom de domaine qualifié)");
define("PQL_LANG_MAILHOST_NONE", "aucun");
define("PQL_LANG_MAILHOST_CHANGE", "Modifier l'adresse du serveur de mail");
define("PQL_LANG_MAILHOST_CHANGE_OK", "Serveur de mail modifié avec succès pour %mailhost%");
define("PQL_LANG_MAILHOST_CHANGE_FAILED", "Echec dans la modification du serveur de mail");
define("PQL_LANG_MAILHOST_DEFAULT", "Standard (entrée DNS)");
define("PQL_LANG_MAILHOST_USERDEFINED", "Défini par l'utilisateur");
define("PQL_LANG_MAILHOST_UNCHANGED", "Serveur de mail inchangé");
define("PQL_LANG_MAILHOST_NOOPTION", "Aucune option est choisie");
define("PQL_LANG_MAILHOST_HELP", "Avec l'option &quot;Standard (entrée DNS)&quot;, la première entrée MX de ce domaine sera utilisée");

// mailmessagestore texts
define("PQL_LANG_MAILMESSAGESTORE_TITLE", "Chemin de la boite email");
define("PQL_LANG_MAILMESSAGESTORE_HELP", "Si vous ne donnez pas un chemin absolu, il sera précédé par la valeur de qmail/control/ldapmessagestore. '..' n'est pas autorisé.");
define("PQL_LANG_MAILMESSAGESTORE_ABSOLUTE_NOTALLOWED", "Les chemins absolus sont interdits par la configuration");
define("PQL_LANG_MAILMESSAGESTORE_NONE", "aucun");
define("PQL_LANG_MAILMESSAGESTORE_CHANGE", "Modifier le chemin de la boite email");
define("PQL_LANG_MAILMESSAGESTORE_CHANGE_OK", "Chemin de la boite email modifié avec succès pour %path%");
define("PQL_LANG_MAILMESSAGESTORE_CHANGE_FAILED", "Echec dans la modification du chemin de la boite email");
define("PQL_LANG_MAILMESSAGESTORE_UNCHANGED", "Chemin de la boite email inchangé");

?>
