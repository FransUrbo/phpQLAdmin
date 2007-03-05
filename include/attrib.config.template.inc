<?php
// attribute plugin for phpQLAdmin/User Templates
// $Id: attrib.config.template.inc,v 1.1 2007-03-05 10:21:08 turbo Exp $

// {{{ attribute_check_templates(void)
function attribute_check_templates() {
  global $LANG;
  $error = false;
  $error_text = array();

  if($_REQUEST["submit"]) {
	if(!$_REQUEST["template_name"]) {
	  $error = true;
	  $error_text["template_name"] = $LANG->_('Missing');
	}
	
	if(!$_REQUEST["template_desc_short"]) {
	  $error = true;
	  $error_text["template_desc_short"] = $LANG->_('Missing');
	}
  } else
	$error = true;

  if($error)
	return($error_text);
}
// }}}

// {{{ attribute_print_form_templates(errors)
function attribute_print_form_templates($error_text) {
  global $LANG;
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
    <input type="hidden" name="rootdn" value="<?=urlencode($_REQUEST["rootdn"])?>">
    <br>
    <input type="submit" value="Create">
  </form>
<?php  
}
// }}}

// {{{ attribute_save_templates(void)
function attribute_save_templates() {
  global $_pql;

  $templates = $_pql->get_dn(pql_get_define("PQL_CONF_SUBTREE_TEMPLATES").','.$_SESSION["BASE_DN"][0],
							 '(&(objectClass=organizationalUnit)(ou=*))', 'BASE');
  if(!is_array($templates)) {
	// Create the organizationalUnit leading up to the templates...
	if(!pql_unit_add($_REQUEST["rootdn"], pql_get_define("PQL_CONF_SUBTREE_TEMPLATES")))
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
  $dn .= ",".pql_get_define("PQL_CONF_SUBTREE_TEMPLATES").",".$_REQUEST["rootdn"];

  // Add the template to the database.
  if(!$_pql->add($dn, $entry, 'template', 'config_template_add/template'))
	$msg = pql_complete_constant($LANG->_("Failed to create template %template%"),
								 array('template' => $_REQUEST["template_name"]));
  else
	$msg = pql_complete_constant($LANG->_("Successfully created template %template%"),
								 array('template' => $_REQUEST["template_name"]));

  attribute_forward($msg);
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
