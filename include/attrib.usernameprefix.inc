<?php
// Attribute plugin for
// userNamePrefix/userNamePrefixLength
// $Id: attrib.usernameprefix.inc,v 2.16.10.1 2006-11-27 13:00:55 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type = '') {
    global $error, $LANG;

    // is typed in ?
    if($_REQUEST["attrib"] == "") {
		$error[$_REQUEST["attrib"]] = $LANG->_('Missing');
		return false;
    }
    
	return true;
}
// }}}

// {{{ attribute_print_form(type)
function attribute_print_form($type = "modify") {
    global $error, $LANG;

    $attrib = $_REQUEST["attrib"];
	if(empty($_REQUEST[$attrib]) and ($attrib == 'usernameprefixlength'))
	  $_REQUEST[$attrib] = 4;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Username prefix to use when automaticly creating username in domain %domain%'), array('domain' => urldecode(pql_format_urls($_REQUEST["orgname"])))); ?></th>
        <tr class="<?php pql_format_table(); ?>">
<?php if($attrib == 'usernameprefix') { ?>
          <td class="title"><?=$LANG->_('Username prefix')?></td>
<?php } elseif($attrib == 'usernameprefixlength') { ?>
          <td class="title"><?=$LANG->_('Length of suffix')?></td>
<?php } ?>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
<?php if($attrib == 'usernameprefix') { ?>
            <input type="text" name="<?=$attrib?>" value="<?=$_REQUEST[$attrib]?>" size="50">
<?php } elseif($attrib == 'usernameprefixlength') { ?>
            <input type="text" name="<?=$attrib?>" value="<?=$_REQUEST[$attrib]?>" size="10">
<?php } ?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td>
            <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
            <table>
<?php if($attrib == 'usernameprefix') { ?>
              <?=$LANG->_('The username will be created with this as prefix, and a incrementing number after it when automaticly creating a username (uid)')?>
<?php } elseif($attrib == 'usernameprefixlength') { ?>
              <?=$LANG->_('This is the length of the automaticly generated numbers after the prefix')?>
<?php } ?>
            </table>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="2">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="domain" value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn" value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
	global $error, $attrib, $$attrib, $domain, $rootdn, $_pql, $view, $LANG;

	$attrib = $_REQUEST["attrib"];

	if($attrib == 'usernameprefix') {
		$lang = "username prefix";
	} elseif($attrib == 'usernameprefixlength') {
		$lang = "length of suffix";
	}
	
    switch($type) {
      case "add":
      case "modify":
	  case "delete":
		if($type == 'delete')
		  $_REQUEST[$attrib] = '';

		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["domain"], $attrib, 1, $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_($lang),
											 'new'  => $_REQUEST[$attrib]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_($lang))) . ": ".ldap_error($_pql->ldap_linkid);

		if(!pql_get_define("PQL_CONF_DEBUG_ME"))
		  attribute_forward($msg);
		break;
		
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
