<?php
// Add a user template
// $Id: config_template_add.php,v 2.6.6.1 2006-11-27 13:00:40 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");

require($_SESSION["path"]."/include/pql_config.inc");
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

include($_SESSION["path"]."/include/attrib.config.inc");
include($_SESSION["path"]."/header.html");

$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
// }}}

// {{{ Verify all submitted values
if($_REQUEST["submit"]) {
	$error = false;
	$error_text = array();
	
	if(!$_REQUEST["template_name"]) {
		$error = true;
		$error_text["template_name"] = $LANG->_('Missing');
	}

	if(!$_REQUEST["template_desc_short"]) {
		$error = true;
		$error_text["template_desc_short"] = $LANG->_('Missing');
	}
} else {
  $error = true;
}
// }}}

if(($error == 'true')) {
  // {{{ Show the input form
?>
  <span class="title1"><?=$LANG->_('Create a user template')?></span>

  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Add user template'); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Template name')?></td>
          <td><?=pql_format_error_span($error_text["template_name"]); ?><input type="text" name="template_name" size="15" value="<?=$_REQUEST["template_name"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title" align="right"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><?=$LANG->_('This is for internal references only. Please keep it as short as possible!')?></td>
        </tr>

        <!-- ------------------------------------- -->

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Short description')?></td>
          <td><?=pql_format_error_span($error_text["template_desc_short"]); ?><input type="text" name="template_desc_short" size="40" value="<?=$_REQUEST["template_desc_short"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title" align="right"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><?=$LANG->_('This is what will end up in the account type selector when creating a user. Please keep it resonable short.')?></td>
        </tr>

        <!-- ------------------------------------- -->

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Long description')?></td>
          <td><?=pql_format_error_span($error_text["template_desc_long"]); ?><textarea name="template_desc_long" cols="40" rows="5" value="<?=$_REQUEST["template_desc_long"]?>"></textarea>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title" align="right"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><?=$LANG->_("This is what will end up as a description on the same page as the short descriptor at the bottom of the page. Don't write a TO long description, or you'll end up with a novell :)")?></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="submit">
    <input type="hidden" name="action" value="add">
    <input type="hidden" name="rootdn" value="<?=$url["rootdn"]?>">
    <br>
    <input type="submit" value="Create">
  </form>
<?php  
// }}}
} else {
  // {{{ No errors. We're good to go!
  $templates = pql_get_dn($_pql->ldap_linkid, 'ou=Templates,'.$_SESSION["BASE_DN"][0],
						  '(&(objectClass=organizationalUnit)(ou=*))', 'BASE');
  if(!is_array($templates)) {
	// Create the organizationalUnit leading up to the templates...
	$entry = array();
	$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = 'organizationalUnit';
	$entry[pql_get_define("PQL_ATTR_OU")] = "Templates";

	$dn = "ou=Templates,".$_REQUEST["rootdn"];
	if(!pql_write_add($_pql->ldap_linkid, $dn, $entry, 'template', 'config_template_add/ou'))
	  die("pql_write_add(ou): failure<br>");
  }

  // Create the template object 'LDIF'
  $entry = array();
  $entry[pql_get_define("PQL_ATTR_USER_TEMPLATE_NAME")]       = lc($_REQUEST["template_name"]);
  $entry[pql_get_define("PQL_ATTR_USER_TEMPLATE_DESC_SHORT")] = $_REQUEST["template_desc_short"];
  $entry[pql_get_define("PQL_ATTR_USER_TEMPLATE_DESC_LONG")]  = $_REQUEST["template_desc_long"];
  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")]              = 'phpQLAdminUserTemplate';

  // Generate the DN
  $dn  = pql_get_define("PQL_ATTR_USER_TEMPLATE_NAME")."=".lc($_REQUEST["template_name"]);
  $dn .= ",ou=Templates,".$_REQUEST["rootdn"];

  // Add the template to the database.
  if(!pql_write_add($_pql->ldap_linkid, $dn, $entry, 'template', 'config_template_add/template'))
	die("pql_write_add(template): failure<br>");

  $url =  "config_detail.php?view=template";
  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
	die("<b>$url</b>");
  } else
	pql_header($url);
// }}}
}
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
