<?php
// Attribute plugin for
// jpegPhoto
// $Id: attrib.photo.inc,v 2.1.6.1 2006-11-27 13:00:55 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type = '') {
    global $error, $LANG;

    // is typed in ?
    if(empty($_REQUEST["attrib"])) {
		$error[$_REQUEST["attrib"]] = $LANG->_('Missing');
		return false;
    }
    
	return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
  return true;
}
// }}}

// {{{ attribute_print_form(type)
function attribute_print_form($type = "modify") {
    global $error, $LANG;
    $attrib = $_REQUEST["attrib"];
?>
  <form enctype="multipart/form-data" action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add JPEG photo for user %user%'), array('user' => $_REQUEST["user"])); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Filename')?></td>
          <td>
            <input type="file" name="<?=$attrib?>" size="50">
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title" align="right"><img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;</td>
          <td><?=$LANG->_('Leave empty to remove photo')?></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="2">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="domain" value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn" value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"   value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Upload')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
	global $_pql, $LANG;
	$attrib = $_REQUEST["attrib"];

    switch($type) {
      case "add":
      case "modify":
	  case "delete":
		if($type == 'delete')
		  $_REQUEST[$attrib] = '';
		else {
		  $fp = fopen($_REQUEST[$attrib]["tmp_name"], "r");
		  if($fp) {
			$data = fread($fp, $_REQUEST[$attrib]["size"]);
			fclose($fp);

			$entry[$attrib] = $data;
		  } else
			// No file -> remove attribute
			$entry[$attrib] = array();
		}

		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], '', 1, $entry))
		  $msg = $LANG->_('Successfully changed JPEG photo');
		else
		  $msg = $LANG->_('Failed to change JPEG photo');

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
