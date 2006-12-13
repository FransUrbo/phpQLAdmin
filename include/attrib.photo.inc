<?php
// Attribute plugin for
// jpegPhoto
// $Id: attrib.photo.inc,v 2.4 2006-12-13 11:33:39 turbo Exp $

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
            <input type="file" name="<?=$attrib?>" size="70">
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
		if(($type == 'delete') or ($_FILES[$attrib]["size"] == 0))
		  $_REQUEST[$attrib] = '';
		else {
			// In PHP versions earlier than 4.1.0, $HTTP_POST_FILES should be used instead
			// of $_FILES.
			$uploaddir = 'tmp/';
			$uploadfile = $uploaddir . basename($_FILES[$attrib]['name']);
			
			if (move_uploaded_file($_FILES[$attrib]['tmp_name'], $uploadfile)) {
				if(pql_get_define("PQL_CONF_DEBUG_ME"))
				  echo "File is valid, and was successfully uploaded.\n";
				
				$fp = fopen($uploadfile, "r");
				if($fp) {
					$data = fread($fp, $_FILES[$attrib]["size"]);
					fclose($fp);

					// Remove the photo file!
					if(file_exists($uploadfile)) {
						unlink($uploadfile);
					}

					$entry[$attrib] = $data;
				} else
				  // No file -> remove attribute
				  $entry[$attrib] = array();
			} else {
				// Could not move/receive file (for some reason).
				echo "Possible file upload attack/problem!\n";
				echo "See <a href='http://se2.php.net/manual/en/features.file-upload.errors.php' target='_top'>";
				echo "the PHP Manual</a> for possible problem/solution<br>";
			}
		}

		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], '', 1, $entry))
		  $msg = $LANG->_('Successfully changed JPEG photo');
		else
		  $msg = $LANG->_('Failed to change JPEG photo');
		
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
