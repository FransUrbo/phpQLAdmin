<?php
// Attribute plugin for
// defaultPasswordScheme
// $Id: attrib.defaultpasswordscheme.inc,v 2.11.6.1 2004-05-25 13:57:51 turbo Exp $

function attribute_check($type = '') {
    global $error, $LANG;

    // is typed in ?
    if(empty($_REQUEST["attrib"])) {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }
    
	return true;
}

function attribute_print_form($type = "modify") {
    global $url, $error, $LANG;

	$attrib = $_REQUEST["attrib"];
?>
  <form action="<?$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Password scheme to use if not specifically specified')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default password scheme')?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <select name="<?=$attrib?>">
<?php // Convert the array to a index
	  $schemes = pql_split_oldvalues(pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]));
	  foreach($schemes as $scheme) {
?>
              <option value="<?=$scheme?>"><?=$scheme?></option>
<?php } ?>
            </select>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="2">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="domain" value="<?=$url["domain"]?>">
    <input type="hidden" name="rootdn" value="<?=$url["rootdn"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];

    switch($type) {
      case "add":
      case "modify":
	  case "delete":
		if($type == 'delete')
		  $_REQUEST[$attrib] = '';

		if(pql_domain_set_value($_pql->ldap_linkid, $_REQUEST["domain"], $attrib, $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
									   array('what'   => $LANG->_('default password scheme'),
											 'domain' => pql_maybe_decode(urldecode($_REQUEST["domain"]))));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what% for domain %domain%'),
									   array('what'   => $LANG->_('default password scheme'),
											 'domain' => pql_maybe_decode(urldecode($_REQUEST["domain"])))) . ":&nbsp;".ldap_error($_pql->ldap_linkid);
	
		attribute_forward($msg);
		break;
		
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
