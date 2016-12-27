﻿/*
  ==============================================================================

    EditAndPreview.h
    Created: 4 Sep 2016 12:28:00am
    Author:  SwingCoder

  ==============================================================================
*/

#ifndef EDITANDPREVIEW_H_INCLUDED
#define EDITANDPREVIEW_H_INCLUDED

class SetupPanel;
class EditorForMd;

//==============================================================================
/** Blank in initial, it'll show edit-mode or preview-mode base on user's click.

    In edit-mode, a textEditor on the left, a propertiesPanel on the right,
    between the two is the stretched-layoutBar.

    The whole area will be a WebComponent when in preview-mode, which covered the 
    textEditor and the propertiesPanel.
*/
class EditAndPreview : public Component,
                       private TextEditor::Listener,
                       private Timer
{
public:
    EditAndPreview();
    ~EditAndPreview();
    
    void paint (Graphics&) override {}
    void resized() override;

    void editNewDoc (const ValueTree& docTree);
    void editCurrentDoc()                                 { editNewDoc (docTree); }    
    void previewDoc (const ValueTree& docTree);
    void previewCurrentDoc()                              { previewDoc (docTree); }

    TextEditor* getEditor() const                         { return editor; }
    const File& getCurrentDocFile() const                 { return docFile;}

    const File createMatchedHtmlFile();

    void projectClosed();
    const bool saveCurrentDocIfChanged();

    void setProjectProperties (ValueTree& projectTree);
    void setDirProperties (ValueTree& dirTree);
    void setDocProperties (ValueTree& docTree);
    
private:
    //=========================================================================
    void editorAndWebInitial();

    virtual void textEditorTextChanged (TextEditor&) override;
    virtual void timerCallback() override;

    //=========================================================================
    File docFile = File::nonexistent;
    ValueTree docTree;

    bool docHasChanged = false;
    bool needCreateHtml = false;
    String currentContent;
    
    ScopedPointer<TextEditor> editor;
    ScopedPointer<WebBrowserComponent> webView;
    ScopedPointer<SetupPanel> setupPanel;

    StretchableLayoutManager layoutManager;
    ScopedPointer<StretchableLayoutResizerBar> layoutBar;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditAndPreview)
};

//=========================================================================
class EditorForMd : public TextEditor
{
public:
    EditorForMd (const File& docFile_) : docFile(docFile_) { }
    ~EditorForMd () { }

    virtual void addPopupMenuItems (PopupMenu& menuToAddTo, const MouseEvent* mouseClickEvent) override;
    virtual void performPopupMenuAction (int menuItemID) override;

    // for solve the TAB key (somehow it can't input spaces, instead of the caret doesn't move)
    bool keyPressed (const KeyPress& key) override;

private:
    const File& docFile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorForMd)
};

#endif  // EDITANDPREVIEW_H_INCLUDED