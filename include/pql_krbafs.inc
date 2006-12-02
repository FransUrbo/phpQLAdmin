<?php
// ----------------------------
// pql_ezmlm.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_krbafs.inc,v 1.4 2006-12-16 12:02:15 turbo Exp $

// Requires the OpenAFS JavaAPI
// http://grand.central.org/twiki/bin/view/AFSLore/JavaAPI

// Kerberos V functions
// {{{ kadmin_init(linkid)
function kadmin_init($linkid) {
  // Get the administrator's Kerberos V principal
  $principal = pql_get_attribute($linkid, $_SESSION["USER_DN"], pql_get_define("PQL_ATTR_KRB5PRINCIPALNAME"));
  
  // Get a connection/authenticate against the Kerberos V server
  $handle = kadm5_init_with_password(pql_get_define("PQL_CONF_KRB5_ADMIN_SERVER"),
									 pql_get_define("PQL_CONF_KRB5_REALM"),
									 $principal, $_SESSION["USER_PASS"]);

  return($handle);
}
// }}}

// {{{ kadmin_ank(linkid, principal, pass)
function kadmin_ank($linkid, $principal, $pass) {
  if(extension_loaded('kadm5') and function_exists("kadm5_init_with_password")) {
	$handle = kadmin_init($linkid);

	$attributes = KRB5_KDB_REQUIRES_PRE_AUTH | KRB5_KDB_DISALLOW_PROXIABLE;
	$options    = array(KADM5_PRINC_EXPIRE_TIME		=> 0,
						KADM5_POLICY				=> "default",
						KADM5_ATTRIBUTES			=> $attributes);

	if(!pql_get_define("PQL_CONF_DEBUG_ME")) {
	  if(!kadm5_create_principal($handle, $principal, $pass, $options))
		return(false);
	} else
	  echo "kadm5_create_principal(handle, $principal, $pass, $options)<br>";

	// Destroy (disconnect) from the Kerberos V server
	kadm5_destroy($handle);
  }

  return(true);
}
// }}}

// {{{ kadmin_delprinc(linkid, principal)
function kadmin_delprinc($linkid, $principal) {
  if(extension_loaded('kadm5') and function_exists("kadm5_init_with_password")) {
	$handle = kadmin_init($linkid);
	if(!pql_get_define("PQL_CONF_DEBUG_ME")) {
	  if(!kadm5_delete_principal($handle, $principal))
		return(false);
	} else
	  echo "kadm5_delete_principal(handle, $principal)<br>";

	// Destroy (disconnect) from the Kerberos V server
	kadm5_destroy($handle);
  }

  return(true);
}
// }}}

// {{{ kadmin_cpw(linkid, principal, pass)
function kadmin_cpw($linkid, $principal, $pass) {
  if(extension_loaded('kadm5') and function_exists("kadm5_init_with_password")) {
	$handle = kadmin_init($linkid);

	// Change password
	if(!pql_get_define("PQL_CONF_DEBUG_ME")) {
	  if(!kadm5_chpass_principal($handle, $principal, $pass))
		return(false);
	} else
	  echo "kadm5_chpass_principal(handle, $principal, $pass)<br>";

	// Destroy (disconnect) from the Kerberos V server
	kadm5_destroy($handle);
  }

  return(true);
}
// }}}

function kadmin_modprinc() {
}

// {{{ kadmin_get_principal(linkid, dn)
function kadmin_get_principal($linkid, $dn) {
  if(extension_loaded('kadm5') and function_exists("kadm5_init_with_password")) {	
	$password = pql_get_attribute($linkid, $dn, pql_get_define("PQL_ATTR_PASSWD"));

	if(eregi('SASL', $password))
	  $principal = eregi_replace("{SASL}", "", $password);
	elseif(eregi('KERBEROS', $password))
	  $principal = eregi_replace("{KERBEROS}", "", $password);
	
	if($principal)
	  return($principal);
  }
  
  return(false);
}
// }}}

// AFS functions
function aklog() {
}

function unlog() {
}

function createuser() {
}

function deleteuser() {
}

function createvolume() {
}

function removevolume() {
}

function setacl() {
}

function setquota() {
}


/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>

