<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-GB">

  <head>
    <title></title>
    
   <link type="text/css" rel="stylesheet" href="css/smoothness/jquery-ui-1.8.16.custom.css" />
    
     
    <link rel="shortcut icon" href="/favicon.ico" type="image/x-icon" />
    <script type="text/javascript" src="js/jquery/jquery-1.7.js"></script>
    <script type="text/javascript" src="js/jquery/jquery-ui-1.8.13.custom.min.js"></script>
    
    
    <!-- Canvas js -->
    <script type="text/javascript" src="js/resize-columns.js"></script>
    <script src="js/jquery.rdfquery.rdfa.min-1.1.js" type="text/javascript"></script>
    <script src="js/jquery.rdf.turtle.js" type="text/javascript"></script>
    
    <script src="js/jquery.touchSwipe-1.2.4.js" type="text/javascript"></script>
    <script src="js/jquery.jplayer.min.js" type="text/javascript"></script>
    
    <script src="js/raphael.js" type="text/javascript"></script>
    <script src="js/scale.raphael.js" type="text/javascript"></script>
    <script src="js/uuid.js" type="text/javascript"></script>
    
    <script type="text/javascript" src="http://www.google.com/jsapi"></script>
    
    <script src="stable/islandora_image_annotation_init.js" type="text/javascript"></script>
    <script src="stable/sc_ui.js" type="text/javascript"></script>
    <script src="stable/sc_utils.js" type="text/javascript"></script>
    <script src="stable/sc_pastebin.js" type="text/javascript"></script>
    <script src="stable/sc_rdf.js" type="text/javascript"></script>
    <script src="stable/sc_rdfjson.js" type="text/javascript"></script>
    <script src="stable/sc_create.js" type="text/javascript"></script>
    <script src="stable/sc_gdata.js" type="text/javascript"></script>
        
    <!-- Canvas css -->
    <link rel="stylesheet" href="css/sc.css" type="text/css" />
    <link rel="stylesheet" href="css/emic_canvas.css" type="text/css" />
      
    <!-- CWRC stylesheets -->
    <link type="text/css" rel="stylesheet" href="css/screen.css" />
    <link type="text/css" rel="stylesheet" href="css/style.css" />


  </head>
  <body>
    <!-- Header -->
    <div id="header">
      <div id="page_selector">Loading....</div>
      <div id="header-inner">
        <div class="header-nav">
          <a href="" id="page-prev"></a>
          <a href="" id="page-next"></a>
        </div>
        <h1></h1>
      </div>
      
    </div>
    <!-- Body -->
    <div class="colmask threecol">
        <div class="colleft">
          <div class="col2">
            <!-- Tabs -->
            <div id="tabs">
              <ul>
                <!--<li><a href="#entities">Entities</a></li>
                <li><a href="#structure">Structure</a></li>-->
                <li id="annotation_tab"><a href="#image-annotations">Image Annotations</a></li>
              </ul>
              <!-- Entities Panel 
              <div id="entities">
                <div id="sortBy">
                  <span>Sort By</span>
                  <input type="radio" id="sequence" name="sortBy" checked="checked"><label for="sequence">Sequence</label></input>
                  <input type="radio" id="category" name="sortBy"><label for="category">Category</label></input>
                </div>
                <ul></ul>
              </div>-->
              <!-- Structure Panel 
              <div id="structure">
                <div id="tree"></div>
              </div>-->
              <!-- Image Annotations Panel -->
              <div id="image-annotations">
                <div id="comment_annos_block" class="dragBlock"></div>
              </div>
            </div>
          </div>
        </div>
        <div id="colright" class="colright">
          <!--<div class="col1">-->
            <!-- Text Annotation 
            <div class="text-annotation-wrapper">
              <form method="post" action="">
                <textarea id="editor" name="editor" class="tinymce"></textarea>
              </form>
            </div>-->
          <!--</div>-->
          <!-- Column Separator -->
          <!--<div id="column-separator"></div>-->
          <div class="col1">
            <!-- Image annotation -->
            <button id="create_annotation" class="menu_button">Annotate</button>
            <div class="image-annotation-wrapper">

              <!-- Persist a single player and build new interface to it -->
              <div id="canvas-body-wrapper" style="width: 100%; height: 800px;"><div id="canvas-body">
          
                <ul class="menu_body" id="show_body">
                  <li class="show_sort" id="li_comment">
                    <span class="ui-icon ui-icon-arrowthick-2-n-s"></span>
                    <span style="margin-left:10px">Commentary:</span>
                    <span style="float:right"><input id="check_show_comment" type="checkbox" checked="true"></input> </span>
                  </li>
          
                  <li class="show_sort" id="li_audio">
                    <span class="ui-icon ui-icon-arrowthick-2-n-s"></span>
                    <span style="margin-left:10px">Audio: </span>
                    <span style="float:right"><input id="check_show_audio" type="checkbox" checked="true"></input></span>
                    <br/>
                    <span>Volume:</span>
                    <div id="slider_volume" style="height:8px;"></div>
                  </li>
                  <li class="show_sort" id="li_text">
                    <span class="ui-icon ui-icon-arrowthick-2-n-s"></span>
                    <span style="margin-left:10px">Texts: </span>
                    <span style="float:right"><input id="check_show_text" type="checkbox" checked="true"></input></span>
                  </li>
                  <li class="show_sort" id="li_detailImg">
                    <span class="ui-icon ui-icon-arrowthick-2-n-s"></span>
                    <span style="margin-left:10px">Detail Images: </span>
                    <span style="float:right"><input id="check_show_detailImg" type="checkbox" checked="true"></input></span>
                  </li>
                  <li class="show_sort" id="li_baseImg">
                    <span class="ui-icon ui-icon-arrowthick-2-n-s"></span>
                    <span style="margin-left:10px">Base Images:</span>
                    <span style="float:right"><input id="check_show_baseImg" type="checkbox" checked="true"></input>
                  </li>
                </ul>
          
                <ul class="menu_body" id="jump_body">
                </ul>
          
                <ul class="menu_body" id="view_body">
                  <li>Show Image Selection: <span style="float:right"><input id="check_view_imgSel" type="checkbox"></input></li>
                  <li>Number of Folios: <span style="float:right" id="viewNumCanvas">1</span>
                    <div id="slider_folios" style="height:8px;"></div></li>
                  <li>Show Zoom Button: <span style="float:right"><input id="check_view_zpr" type="checkbox"></input></li>
                  <li>Show Canvas URI: <span style="float:right"><input id="check_view_uri" type="checkbox"></input></li>
                </ul>
          
                <!--  Wrapper to create Canvas divs in -->
                <div id="canvases"></div>
          
                <!--  Wrapper to create SVG divs in -->
                <div id="svg_wrapper"></div>
          
                <!--  Wrapper to create annotations in, then reposition -->
                <div id="annotations"></div>
          
                <!-- Progress bar -->
                <div id="loadprogress"></div>
                        
              <!--  At least one visible image needed for GData transport -->
             
            
            </div>
            </div>
            </div>
          </div>
        </div>
    </div>

    <div id="dialogs" width="500">
          
      <!-- Image annotation box -->
      <div id="create_annotation_box" style="width: 380px" class="dragBox ui-dialog ui-widget ui-corner-all ui-draggable ui-resizable">
        <div id="create_annos_header" class="dragHead ui-dialog-titlebar ui-widget-header ui-corner-all">
          <span>Annotate</span>
          
        </div>
        <!-- Annotation shapes -->
        <div style="display:inline; margin-top: 3px; padding-left: 5px;">
          <img id="annoShape_rect" class="annoShape" src="imgs/draw_rect.png" style="padding-left: 2px; padding-top: 1px;"/>
          <img id="annoShape_circ" class="annoShape" src="imgs/draw_circ.png" style="padding-left: 1px;"/>
          <img id="annoShape_poly" class="annoShape" src="imgs/draw_poly.png" style="padding: 2px;"/>
          <hr style="margin: 0px; padding: 0px; height: 1px;"/>
        </div>
        <div id="create_annos_block" class="dragBlock">
          <!-- Annotation Title -->
          <div class="element-wrap">
            <label for="anno_title">Title:</label>
            <input id="anno_title" type="text" size="28"></input>
          </div>
          <!-- Annotation Type - to be removed -->
          <span style="float:right" hidden ="true"><select id="anno_type">
              <option value="comment">Commentary</option>
              <option value="transcription">Transcription</option>
          </select></span>
          <!-- Annotation textarea -->
          <div class="element-wrap">
            <label for="anno_text">Annotation:</label>
            <textarea id="anno_text" cols="40" rows="5"></textarea>
          </div>
          <!-- Services - to be removed -->
          <span style="width:200px;margin:0px;padding:0px;float:left">
            <ul id="create_body" style="width: 200px; list-style:none;font-size:10pt;margin:0;padding:0;">
            </ul>
          </span>
          <!-- Cancel/Save buttons -->
          <span style="float: right; padding-top: 3px;">
            <button class="diabutton" id="cancelAnno">Cancel</button>
            <button class="diabutton" id="saveAnno">Save</button>
          </span>
        </div>
      </div>
    </div>

     <!-- Footer -->
    <div id="footer">
      <div class="shared-canvas-logo" style="font-size:8pt">
                <img height="15" src="imgs/small-logo.png" style="padding: 0px; margin: 0px; border: 0px; border-top: 2px;" />
                Powered by SharedCanvas
              </div></div>
  </body>
</html>
