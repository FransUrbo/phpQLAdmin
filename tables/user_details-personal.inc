<?php $photo = $_pql->get_attribute($_GET["user"], pql_get_define("PQL_ATTR_PHOTO")); ?>
  <table cellspacing="0" cellpadding="0" border="0">
    <th>
      <tr>
        <td valign="top" width="350">
          <table cellspacing="0" cellpadding="3" border="0">
            <th colspan="3" align="left"><?php echo $LANG->_('Personal details')?></th>
              <tr class="<?php pql_format_table(); ?>">
                <td class="title"><?php echo $LANG->_('Company/Organization')?></td>
                <td><?php if($o) { echo $o; } else { echo $LANG->_('Not set'); }?></td>
                <td><?php echo $o_link?></td>
              </tr>
        
              <tr class="<?php pql_format_table(); ?>">
                <td class="title"><?php echo $LANG->_('Title')?></td>
                <td><?php if($personaltitle) { echo $personaltitle; } else { echo $LANG->_('Not set'); }?></td>
                <td><?php echo $personaltitle_link?></td>
              </tr>
        
              <tr class="<?php pql_format_table(); ?>">
                <td class="title"><?php echo $LANG->_('Room number')?></td>
                <td><?php if($roomnumber) { echo $roomnumber; } else { echo $LANG->_('Not set'); }?></td>
                <td><?php echo $roomnumber_link?></td>
              </tr>
        
              <tr class="<?php pql_format_table(); ?>">
                <td class="title"><?php echo $LANG->_('Telephone number')?> (<?php echo $LANG->_('Work')?>)</td>
                <td><?php if($telephonenumber) { echo $telephonenumber; } else { echo $LANG->_('Not set'); }?></td>
                <td><?php echo $telephonenumber_link?></td>
              </tr>
        
              <tr class="<?php pql_format_table(); ?>">
                <td class="title"><?php echo $LANG->_('Telephone number')?> (<?php echo $LANG->_('Home')?>)</td>
                <td><?php if($homephone) { echo $homephone; } else { echo $LANG->_('Not set'); }?></td>
                <td><?php echo $homephone_link?></td>
              </tr>
        
              <tr class="<?php pql_format_table(); ?>">
                <td class="title"><?php echo $LANG->_('Telephone number')?> (<?php echo $LANG->_('Mobile')?>)</td>
                <td><?php if($mobile) { echo $mobile; } else { echo $LANG->_('Not set'); }?></td>
                <td><?php echo $mobile_link?></td>
              </tr>
        
              <tr class="<?php pql_format_table(); ?>">
                <td class="title"><?php echo $LANG->_('Telephone number')?> (<?php echo $LANG->_('Pager')?>)</td>
                <td><?php if($pager) { echo $pager; } else { echo $LANG->_('Not set'); }?></td>
                <td><?php echo $pager_link?></td>
              </tr>
        
              <tr class="<?php pql_format_table(); ?>">
                <td class="title"><?php echo $LANG->_('Street address')?> (<?php echo $LANG->_('Home')?>)</td>
                <td><?php if($homepostaladdress) { echo $homepostaladdress; } else { echo $LANG->_('Not set'); }?></td>
                <td><?php echo $homepostaladdress_link?></td>
              </tr>
            </th>
          </table>
        </td>

        <td valign="top">
          <table cellspacing="0" cellpadding="3" border="0">
            <th align="left" valign="top"><?php echo $LANG->_('User picture')?></th>
<?php  if(!empty($photo)) { ?>
              <tr class="<?php pql_format_table(); ?>" align="left" valign="top">
                <td>
<?php	$filename = "tmp/".$_GET["user"].".jpg";

		// Remove the photo!
		if(file_exists($filename)) {
		  unlink($filename);
		}

		// base64 decode the photo
		$photo = base64_decode($photo);

		$fp = fopen($filename, "wb");
		if($fp) {
		  fputs($fp, $photo);
		  fclose($fp);

		  $written_photo = 1;
?>
                  <img src="<?php echo $filename?>" width="250" height="250">
<?php	} ?>
                </td>
              </tr>
<?php  } ?>
              
              <tr class="<?php pql_format_table(); ?>" align="left" valign="top">
<?php  if(empty($photo)) { ?>
                <td class="title" align="center"><a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=<?php echo pql_get_define("PQL_ATTR_PHOTO")?>&user=<?php echo $url["user"]?>&action=add&view=<?php echo $_GET["view"]?>"><?php echo $LANG->_('Add picture')?></a></td>
<?php } else { ?>
                <td class="title" align="center"><a href="user_edit_attribute.php?rootdn=<?php echo $url["rootdn"]?>&domain=<?php echo $url["domain"]?>&attrib=<?php echo pql_get_define("PQL_ATTR_PHOTO")?>&user=<?php echo $url["user"]?>&action=replace&view=<?php echo $_GET["view"]?>"><?php echo $LANG->_('Replace picture')?></a></td>
<?php } ?>
              </tr>
            </th>
          </table>
        </td>
      </tr>
  </table>

  <br><br>
</html>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
