/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import {I18nMixin} from 'chrome://resources/js/i18n_mixin.js';
import {PolymerElement, html} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';
import {BaseMixin} from '../../base_mixin.js';
import {PrefsMixin} from '../../prefs/prefs_mixin.js';

import 'chrome://resources/cr_elements/cr_button/cr_button.m.js';

const AdBlockEditorBase = PrefsMixin(I18nMixin(BaseMixin(PolymerElement)))

class AdBlockEditor extends AdBlockEditorBase {
  static get is() {
    return 'adblock-editor'
  }

  static get template() {
    return html`{__html_template__}`
  }

  static get properties() {
    return {
      value: {
        type: Text,
        observer: 'lineNumberObserver_'
      },
      activeLineNumber_: {
        type: Number,
        value: null
      },
      lines_: Array,
      hasObserved_: {
        type: Boolean,
        value: false
      }
    }
  }

  ready() {
    super.ready()
  }

  lineNumberObserver_() {
    // We run once, on mount
    if (this.hasObserved_) {
      return
    }

    this.updateLineNumbers_()
    this.hasObserved_ = true
  }

  updateLineNumbers_() {
    let currentLC = this.value.split('\n').length
    const lines = []
    
    while (currentLC) {
      lines.push(0)
      currentLC--
    }
    
    this.lines_ = lines
  }

  handleInputChange_(e) {
    this.value = e.target.value
    this.updateLineNumbers_()
  }

  handleOnScroll_(e) {
    this.$.linebox.scrollTop = e.target.scrollTop
  }

  handleOnKeyUp_(e) {
    const keys = ['Enter', 'ArrowUp', 'ArrowDown', 'ArrowLeft', 'ArrowRight', 'Delete', 'Backspace']

    if (!keys.includes(e.key)) {
      return
    }

    this.updateActiveLineNumber_(
      e.target.value, 
      e.target.selectionStart
    )
  }

  handleOnClick_(e) {
    this.updateActiveLineNumber_(
      e.target.value, 
      e.target.selectionStart
    )
  }

  clearActiveLineNumber_() {
    this.activeLineNumber_ = null
  }

  updateActiveLineNumber_(text, caretPos) {
   const lN = (text.substr(0, caretPos).split('\n').length)
   this.activeLineNumber_ = lN
  }

  getClassForHighlight_(id, currentId) {
    if (id+1 === currentId) {
      return 'active'
    }

    return ''
  }

  handleSave_() {
    this.fire('save', { value: this.value })
  }
}

customElements.define(AdBlockEditor.is, AdBlockEditor)
