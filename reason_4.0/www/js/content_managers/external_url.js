function getOptions(item){
	var optionNodes = item.children;
	var options = [];
	for(var x =0; x<optionNodes.length; x+=1){
		var key = optionNodes[x].nodeName;
		if(key.indexOf(":")!= -1){
			key = key.split(":")[0];
		}
		key = key.toLowerCase();
		options.push(key);
	}
	return options;

}

// function addOptions(options){
// 	for(var x=0;x<options.length;x+=1){
// 		var optionElem = document.createElement("OPTION");
// 		optionElem.setAttribute('value',options[x]);
// 		var optionText = document.createTextNode(options[x]);
//  		optionElem.appendChild(optionText);
//  		$('#field_titleElement').append(optionElem);
//  	}
//  }
//
// function displayFeed(event){
// 	if(event != undefined){
// 		var url = event.target.value;
// 	}
// 	else{
// 		var url = $("#urlElement")[0].value;
// 	}
// 	$.get(url, function (data) {
// 		$('#previewElement').html("");
// 		if(url != "" && $(data).find("item").length == 0){
// 			var item = document.createElement("DIV");
// 			item.className = "rssPreviewItem";
// 			var error = document.createElement("P");
// 			var errorCont = document.createTextNode("Error: URL does not contain an RSS feed.");
// 			error.appendChild(errorCont);
// 			item.appendChild(error);
// 			$('#previewElement').append(item);
// 		}
// 		else{
// 			if($(data).find("item").length != 0){
// 				var options = getOptions($(data).find("item")[0]);
// 				//addOptions(options);
// 				var remvOptions = ['title','description','author'];
// 				for(var x =0; x<remvOptions.length; x+=1){
// 					if(options.indexOf(remvOptions[x]) != -1){
// 						options.splice(options.indexOf(remvOptions[x]),1);
// 					}
// 				}
// 			}
// 			$(data).find("item").slice(0,3).each(function () {
// 	        		var el = $(this);
//
// 					var item = document.createElement("DIV");
// 					item.className = "rssPreviewItem";
// 					var title = document.createElement("H3");
// 					var author = document.createElement("P");
// 					author.className = "rssPreviewAuthor";
// 					var desc = document.createElement("P");
// 					var titleText = el.find('title').text();
// 					var authorText = el.find('author').text();
// 					var descText = el.find('description').text();
// 					var titleCont = document.createTextNode(titleText);
// 					if(authorText != ""){
// 						var authorCont = document.createTextNode("By " + authorText);
// 					}
// 					else{
// 						var authorCont = document.createTextNode("");
// 					}
// 					if(descCont != ""){
// 						var descCont = document.createTextNode(descText);
// 					}
// 					else{
// 						var descCont = document.createTextNode("");
// 					}
// 					title.appendChild(titleCont);
// 					author.appendChild(authorCont);
// 					desc.appendChild(descCont);
// 					item.appendChild(title);
// 					item.appendChild(author);
// 					item.appendChild(desc);
//
// 					//dynamic options
// 					for(var x =0; x<options.length; x+=1){
// 							var option = document.createElement("P");
// 							var optionCont = document.createTextNode(options[x] +": " + el.find(options[x]).text());
// 							option.appendChild(optionCont);
// 							item.appendChild(option);
// 					}
//
// 					$('#previewElement').append(item);
// 	    		});
// 			}
// 	});
// }

function displayFeed(){
	for(x=0;x<rssItems.length;x+=1){
		var itemDiv = document.createElement("DIV");
		itemDiv.className = "rssPreviewItem";
		if(x==0){
			itemDiv.className += " top";
		}
		var item = rssItems[x];
		var keys = Object.keys(item);
		for(y=0;y<keys.length;y+=1){
			key = keys[y];
			keyDiv = document.createElement("DIV");
			keyDiv.className = "rssKeySec";
			if(key=="title"){
				var keyValue = document.createElement("H3");
			}
			else{
				var keyValue = document.createElement("P");
			}
			keyValue.className += " rssKeyValue";
			var keyName = document.createElement("P");
			keyName.className = "rssKeyName";
			if(key=="author"){
				keyValue.className += "rssPreviewAuthor";
			}
			console.log(typeof item[key]);
			if(typeof item[key] == 'object'){
				keyValueText = document.createTextNode("Object/Null");
			}
			else{
				keyValueText = document.createTextNode(item[key]);
			}
			keyNameText = document.createTextNode(key);

			keyValue.appendChild(keyValueText);
			keyName.appendChild(keyNameText);

			keyDiv.appendChild(keyName);
			keyDiv.appendChild(keyValue);

			itemDiv.appendChild(keyDiv);
		}
		$('#previewElement').append(itemDiv);
	}
}

$(document).ready(function() {
	var url = $("#urlElement")[0].value;
	if(url != undefined && url != ""){
		$('<tr valign="top" id="previewRow"><td align="right" class="words"><span class="labelText">Preview:</span></td><td align="left" class="element"><div id="previewElement"></div></td></tr>').insertAfter($('#urlRow'));
		displayFeed();
	}
	else{
		console.log(url);
		$("#fieldtitleRow").hide();
		$("#fieldwordsRow").hide();
	}
	//$("#urlElement").blur(displayFeed);
	//$("#urlElement").change(displayFeed);
});
