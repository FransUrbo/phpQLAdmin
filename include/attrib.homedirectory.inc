<?php
// attribute plugin for
// homedirectory (path to homedirectory)
// $Id: attrib.homedirectory.inc,v 2.12.2.3 2003-12-09 16:18:26 dlw Exp $

function attribute_check($type) {
    global $error, $LANG;
    
    // is typed in ?
    if(empty($_POST["homedirectory"])) {
		$error["homedirectory"] = $LANG->_('Missing');
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $_POST["homedirectory"])) {
		$error["homedirectory"] = $LANG->_('Invalid');
		return false;
    }
    
    return true;
}

function attribute_init() {
    $_REQUEST["homedirectory"] = (!empty($_GET["oldvalue"]) ? $_GET["oldvalue"] : "");
}

function attribute_print_form() {
    global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change home directory')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Change home directory')?></td>
          <td><?php echo pql_format_error_span($error["homedirectory"]); ?><input type="text" name="homedirectory" value="<?=$_REQUEST["homedirectory"]?>" size="50"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"   value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="user"     value="<?=$_REQUEST["user"]?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
    global $_pql, $LANG;

    switch($type) {
      case "add":
		// no add operation possible with homedirectory
		break;

      case "modify":
		if($_POST["homedirectory"] == $_POST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Home directory')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_attribute($_pql->ldap_linkid, $_POST["user"], $_POST["attrib"], $_POST["homedirectory"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('Home directory', 4),
											 'new'  => $_POST["homedirectory"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Home directory', 4))) . ": " . ldap_error($_pql->ldap_linkid);
		
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
