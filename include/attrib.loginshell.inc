<?php
// attribute plugin for
// loginshell
// $Id: attrib.loginshell.inc,v 2.16 2004-06-10 05:37:43 turbo Exp $

function attribute_check($type) {
    global $error, $LANG;
    
    // is typed in ?
    if(empty($_REQUEST["loginshell"])) {
		$error["loginshell"] = $LANG->_('Missing');
		return false;
    }

    return true;
}

function attribute_init() {
    $_REQUEST["loginshell"] = (empty($_GET["oldvalue"]) ? "" : $_GET["oldvalue"]);
}

function attribute_print_form() {
    global $error, $LANG;

	// Get all shells we can choose from
    $shells   = pql_get_valid_shells();
	$shells[] = '/bin/false';

	// Get the users existing shell
    $existing_shell = pql_get_attribute($_pql->ldap_linkid, $_GET["user"],
										pql_get_define("PQL_ATTR_LOGINSHELL"));
	if(!empty($existing_shell[0])) {
		$existing_shell = $existing_shell[0];
	} else {
		$existing_shell = '/bin/false';
	}
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Login shell')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Login shell')?></td>
          <td>
            <?php echo pql_format_error_span($error["loginshell"]); ?>

            <select name="loginshell">
<?php 
    foreach($shells as $shell) {
?>
              <option value="<?=$shell?>" <?php
		if($existing_shell == $shell) {
			echo "SELECTED";
		}?>><?=$shell?></option>
<?php
    }
?>
            </select>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?=urlencode($_REQUEST["user"])?>">
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
		// no add operation possible with loginshell
		break;

      case "modify":
		if($_REQUEST["loginshell"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('login shell')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"], $_REQUEST["attrib"], $_REQUEST["loginshell"])) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('login shell'),
											   'new'  => $_REQUEST["loginshell"]));
		} else {
			$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										 array('what' => $LANG->_('login shell'))) . ": " . ldap_error($_pql->ldap_linkid);
		}
		
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
