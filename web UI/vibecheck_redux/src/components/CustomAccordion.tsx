import React, { useState, useRef, useEffect } from 'react';

interface AccordionItemProps {
  title: string;
  children: React.ReactNode;
  isOpen: boolean;
  onClick: () => void;
}

const AccordionItem: React.FC<AccordionItemProps> = ({ title, children, isOpen, onClick }) => {
  const contentRef = useRef<HTMLDivElement>(null);
  const [contentHeight, setContentHeight] = useState(0);

  useEffect(() => {
    if (isOpen && contentRef.current) {
      setContentHeight(contentRef.current.scrollHeight);
    } else {
      setContentHeight(0);
    }
  }, [isOpen]);

  return (
    <div className="border-b border-gray-200 last:border-b-0">
      <button
        className="flex justify-between items-center w-full py-4 px-6 text-left text-sm font-medium text-gray-700 hover:bg-gray-50 focus:outline-none focus:ring-2 focus:ring-blue-500"
        onClick={onClick}
        aria-expanded={isOpen}
      >
        <span>{title}</span>
        <span className={`transform transition-transform duration-200 ${isOpen ? 'rotate-180' : ''}`}>
          ▼
        </span>
      </button>
      <div
        className="overflow-hidden transition-all duration-200 ease-in-out"
        style={{ maxHeight: `${contentHeight}px` }}
        aria-hidden={!isOpen}
      >
        <div ref={contentRef} className="p-6 bg-white">
          {isOpen && children}
        </div>
      </div>
    </div>
  );
};

interface AccordionProps {
  items: {
    title: string;
    content: React.ReactNode;
  }[];
  allowMultiple?: boolean;
}

const CustomAccordion: React.FC<AccordionProps> = ({ items, allowMultiple = false }) => {
  const [openItems, setOpenItems] = useState<boolean[]>(new Array(items.length).fill(false));

  const toggleItem = (index: number) => {
    setOpenItems((prevOpenItems) => {
      if (allowMultiple) {
        const newOpenItems = [...prevOpenItems];
        newOpenItems[index] = !newOpenItems[index];
        return newOpenItems;
      } else {
        return prevOpenItems.map((_, i) => i === index ? !prevOpenItems[index] : false);
      }
    });
  };

  return (
    <div className="border border-gray-200 rounded-lg overflow-hidden">
      {items.map((item, index) => (
        <AccordionItem
          key={index}
          title={item.title}
          isOpen={openItems[index]}
          onClick={() => toggleItem(index)}
        >
          {item.content}
        </AccordionItem>
      ))}
    </div>
  );
};

export default CustomAccordion;