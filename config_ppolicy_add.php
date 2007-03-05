<?php
// Add a Password Policy
// $Id: config_ppolicy_add.php,v 1.1 2007-03-05 10:12:22 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

include($_SESSION["path"]."/include/attrib.config.inc");
include($_SESSION["path"]."/include/attrib.config.ppolicy.inc");
include($_SESSION["path"]."/header.html");
// }}}

// {{{ Forward back to configuration detail page
function attribute_forward($msg) {
  $url =  "config_detail.php?view=ppolicy";
  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
	die("<b>$url</b>");
  } else
	pql_header($url);
}
// }}}

// {{{ Simplicity - with this we can have ONE single foreach() loop to output the whole view!
$table_columns = array(pql_get_define("PQL_ATTR_PPOLICY_NAME")					=> $LANG->_('Password Policy Name'),
					   pql_get_define("PQL_ATTR_PPOLICY_DESC")					=> $LANG->_('Password Policy Description'),
					   pql_get_define("PQL_ATTR_PPOLICY_ATTR")					=> $LANG->_('Password attribute'),
					   pql_get_define("PQL_ATTR_PPOLICY_MINAGE")				=> $LANG->_('Minimum age'),
					   pql_get_define("PQL_ATTR_PPOLICY_MAXAGE")				=> $LANG->_('Maximum age'),
					   pql_get_define("PQL_ATTR_PPOLICY_INHIST")				=> $LANG->_('Numer of old passwords in history'),
					   pql_get_define("PQL_ATTR_PPOLICY_CHKQUAL")				=> $LANG->_('Password control/verification type'),
					   pql_get_define("PQL_ATTR_PPOLICY_MINLEN")				=> $LANG->_('Minimum password length'),
					   pql_get_define("PQL_ATTR_PPOLICY_EXPIRE_WARNING")		=> $LANG->_('Password expiration warning'),
					   pql_get_define("PQL_ATTR_PPOLICY_GRACEAUTHLIMIT")		=> $LANG->_('Number of times to allow expired password'),
					   pql_get_define("PQL_ATTR_PPOLICY_LOCKOUT")				=> $LANG->_('Lock account after failed attempt'),
					   pql_get_define("PQL_ATTR_PPOLICY_LOCKOUT_DURATION")		=> $LANG->_('Time between failed attempt and new attempt'),
					   pql_get_define("PQL_ATTR_PPOLICY_MAXFAILURE")			=> $LANG->_('Maximum failed attempts before lock'),
					   pql_get_define("PQL_ATTR_PPOLICY_FAILURE_COUNT_INTERVAL")=> $LANG->_('Time for old failed attempts to be forgotten'),
					   pql_get_define("PQL_ATTR_PPOLICY_MUST_CHANGE")			=> $LANG->_('Force password change'),
					   pql_get_define("PQL_ATTR_PPOLICY_ALLOW_USER_CHANGE")		=> $LANG->_('Allow user to change password'),
					   pql_get_define("PQL_ATTR_PPOLICY_SAFE_MODIFY")			=> $LANG->_('Force the use of old password when changing'),
					   pql_get_define("PQL_ATTR_PPOLICY_CHECK_MODULE")			=> $LANG->_('Additional password check module'));
// }}}

// {{{ Verify all submitted values and show form or save
$error = attribute_check_ppolicy();
if(isset($error)) {
  // Errors or we've been asked to add a policy - show form.
  attribute_print_form_ppolicy($error);
} else {
  // No errors. We're good to go!
  attribute_save_ppolicy();
}
// }}}
?>
  </body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
